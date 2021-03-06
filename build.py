import os
import subprocess
import sys
import re
import requests



FONT_OUT_NAME="font_spleen"
FONT_URL="https://raw.githubusercontent.com/fcambus/spleen/master/spleen-8x16.bdf"
FONT_MAX_CHAR=0x7e
INCLUDE_LIST_REGEX=re.compile(br"^\s*?((?:\s*#\s*include\s*<[^>]*?>)+)",re.M)
INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$")
REQUIRED_STRUCT_OFFSETS={b"__KERNEL_ARGS":[b"k_sp"],b"__CPUID_INFO":[b"eax",b"ebx",b"ecx",b"edx"]}
REQUIRED_STRUCT_SIZE=[b"__THREAD_DATA"]
SIZEOF_POINTER=8
SIZEOF_UINT32_T=4
SIZEOF_UINT64_T=8



def _sort_inc(m):
	l=[INCLUDE_FILE_REGEX.search(e.strip()).group(1) for e in m.group(1).strip().split(b"\n")]
	o=(b"#include <shared.h>"+(b"\n" if len(l)>1 else b"") if b"shared.h" in l else b"")
	if (len(o)):
		l.remove(b"shared.h")
	return o+b"\n".join([b"#include <"+e+b">" for e in sorted(l)])



for k in os.listdir("build"):
	if (os.path.isdir(f"build/{k}")==True):
		tdl=[f"build/{k}"]
		for r,dl,fl in os.walk(f"build/{k}"):
			tdl=[os.path.join(r,e) for e in dl]+tdl
			for f in fl:
				os.remove(os.path.join(r,f))
		for e in tdl:
			if (e not in ["build/efi","build/kernel","build/libc"]):
				os.rmdir(e)
	else:
		os.remove(f"build/{k}")
if (not os.path.exists("build/efi")):
	os.mkdir("build/efi")
if (not os.path.exists("build/kernel")):
	os.mkdir("build/kernel")
if (not os.path.exists("build/libc")):
	os.mkdir("build/libc")
e_fl=[]
k_fl=[]
l_fl=[]
if (not os.path.exists(f"rsrc/{FONT_OUT_NAME}.c") or not os.path.exists(f"rsrc/include/{FONT_OUT_NAME}.h") or "--reload-rsrc" in sys.argv):
	print(f"Font characters: {FONT_MAX_CHAR+1}\nFont URL: {FONT_URL}\nFont Files: rsrc/include/{FONT_OUT_NAME}.h, rsrc/{FONT_OUT_NAME}.c")
	dt=requests.get(FONT_URL,headers={"Accept":"application/vnd.github.v3+json","User-Agent":"Font Generation"}).content.replace(b"\r",b"\n").split(b"\n")
	i=0
	o=[0 for _ in range(0,(FONT_MAX_CHAR+1)*2)]
	while (i<len(dt)):
		if (dt[i][:9]==b"STARTCHAR"):
			i+=1
			id_=-1
			j=-1
			while (dt[i]!=b"ENDCHAR"):
				if (dt[i][:8]==b"ENCODING"):
					id_=int(dt[i][8:].strip())
					if (id_>FONT_MAX_CHAR):
						while (dt[i]!=b"ENDCHAR"):
							i+=1
						break
				elif (dt[i][:6]==b"BITMAP"):
					j=0
				elif (j!=-1):
					o[id_*2+j//8]|=int(bin(int(dt[i],16))[2:].rjust(8,'0')[::-1],2)<<(8*(j%8))
					j+=1
				i+=1
		i+=1
	with open(f"rsrc/include/{FONT_OUT_NAME}.h","w") as f:
		f.write(f"#ifndef __{FONT_OUT_NAME.upper()}_H__\n#define __{FONT_OUT_NAME.upper()}_H__ 1\n#include <gfx/font.h>\n#include <stdint.h>\n\n\n\nextern const uint64_t {FONT_OUT_NAME.upper()}_DATA[];\n\n\n\nextern Font {FONT_OUT_NAME.upper()};\n\n\n\n#endif\n")
	with open(f"rsrc/{FONT_OUT_NAME}.c","w") as f:
		f.write(f"#include <gfx/font.h>\n#include <stdint.h>\n#include <{FONT_OUT_NAME}.h>\n\n\n\nconst uint64_t {FONT_OUT_NAME.upper()}_DATA[{(FONT_MAX_CHAR+1)*2}]={{\n\t{(','+chr(10)+chr(9)).join(['0x'+hex(e)[2:].rjust(16,'0') for e in o])}\n}};\n\n\n\nFont {FONT_OUT_NAME.upper()}={{\n\t{FONT_MAX_CHAR},\n\t{FONT_OUT_NAME.upper()}_DATA\n}};\n")
src_fl=list(os.walk("rsrc"))+list(os.walk("src"))
asm_d=[]
for r,_,fl in src_fl:
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (f[-2:]==".c" or f[-2:]==".h"):
			print(f"Prettifying C {('Source' if f[-1]=='c' else 'Header')} File: {r+f}")
			with open(r+f,"rb") as rf:
				dt=INCLUDE_LIST_REGEX.sub(_sort_inc,rf.read())
			if (f[-1]=="h"):
				for k in set(REQUIRED_STRUCT_OFFSETS.keys())|set(REQUIRED_STRUCT_SIZE):
					m=re.search(br"struct\s+"+k+br"\s*\{",dt)
					if (m is not None):
						i=m.end()
						off=0
						lc=-1
						while (True):
							if (dt[i:i+1] in b" \t\r\n\v\f"):
								i+=1
								continue
							e=b""
							while (dt[i:i+1]!=b";"):
								e+=dt[i:i+1]
								i+=1
							i+=1
							if (k in REQUIRED_STRUCT_OFFSETS):
								nm=e.split(b" ")[-1].replace(b"[]",b"")
								if (nm in REQUIRED_STRUCT_OFFSETS[k]):
									asm_d+=[f"-D__{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(nm,'utf-8').upper()}_OFFSET__={off}"]
									REQUIRED_STRUCT_OFFSETS[k].remove(nm)
									if (len(REQUIRED_STRUCT_OFFSETS[k])==0 and k not in REQUIRED_STRUCT_SIZE):
										break
							e=e[:-len(e.split(b" ")[-1])].strip()
							c=0
							if (e==b"}"):
								break
							elif (e[-1:]==b"*"):
								c=SIZEOF_POINTER
							elif (e==b"uint32_t"):
								c=SIZEOF_UINT32_T
							elif (e==b"uint64_t"):
								c=SIZEOF_UINT64_T
							else:
								print(f"Unknown sizeof of Type '{str(e,'utf-8')}'!")
								quit()
							if (lc!=-1 and lc<c):
								print(f"Unknown Amount of Padding between {lc}-byte Type and {c}-byte Type!")
								quit()
							off+=c
							lc=c
						if (k in REQUIRED_STRUCT_SIZE):
							asm_d+=[f"-D__{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__={off}"]
							REQUIRED_STRUCT_SIZE.remove(k)
						if (k in REQUIRED_STRUCT_OFFSETS):
							if (REQUIRED_STRUCT_OFFSETS[k]):
								for e in REQUIRED_STRUCT_OFFSETS[k]:
									print(f"Unable to Find Element '{str(e,'utf-8')}' in Structure '{str(k,'utf-8')}'!")
								quit()
							del REQUIRED_STRUCT_OFFSETS[k]
			with open(r+f,"wb") as wf:
				wf.write(dt)
for k in REQUIRED_STRUCT_OFFSETS.keys():
	print(f"Unable to Find Struture '{str(k,'utf-8')}'!")
if (REQUIRED_STRUCT_OFFSETS):
	quit()
for k in REQUIRED_STRUCT_SIZE:
	print(f"Unable to Find Struture '{str(k,'utf-8')}'!")
if (REQUIRED_STRUCT_SIZE):
	quit()
for r,_,fl in src_fl:
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (r[:7]=="src/efi"):
			if (f[-2:]==".c"):
				print(f"Compiling C File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o")
				if (subprocess.run(["bash","-c",f"gcc -Isrc/kernel/include -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -fno-stack-protector -O3 -fpic -fshort-wchar -fno-common -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Werror -c {r+f} -o build/efi/{(r+f)[8:].replace('/',chr(92)+'$')}.o"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/efi/{(r+f)[8:].replace('/',chr(92)+'$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{(r+f)[8:].replace('/',chr(92)+'$')}.o"]
			elif (f[-4:]==".asm"):
				print(f"Compiling ASM File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o")
				if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/efi/{(r+f)[8:].replace('/','$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{(r+f)[8:].replace('/','$')}.o"]
		elif (r[:10]=="src/kernel"):
			if (f[-2:]==".c"):
				print(f"Compiling C File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o","-Isrc/kernel/include","-Irsrc/include","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
			elif (f[-4:]==".asm"):
				print(f"Compiling ASM File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]+asm_d).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
		elif (r[:4]=="rsrc"):
			if (f[-2:]==".c"):
				print(f"Compiling C File (Kernel Resource): {r+f} -> build/kernel/{(r+f).replace('/','$')}.o")
				if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f).replace('/','$')}.o","-Irsrc/include","-Isrc/kernel/include","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f).replace('/','$')}.o"]).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f).replace('/','$')}.o"]
		else:
			if (f[-2:]==".c"):
				print(f"Compiling C File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o","-Isrc/kernel/include","-Isrc/libc/include","-Irsrc","-D__KERNEL__=1"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				print(f"Compiling C File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/libc/{(r+f)[4:].replace('/','$')}.o","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/libc/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
					quit()
				l_fl+=[f"build/libc/{(r+f)[4:].replace('/','$')}.o"]
			elif (f[-4:]==".asm"):
				print(f"Compiling ASM File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-D__KERNEL__=1","-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				print(f"Compiling ASM File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o")
				if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/libc/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						quit()
				l_fl+=[f"build/libc/{(r+f)[4:].replace('/','$')}.o"]
print(f"Linking EFI OS Loader: {', '.join([e.replace(chr(92)+'$','$') for e in e_fl])}")
if (subprocess.run(["bash","-c",f"ld -nostdlib -znocombreloc -fshort-wchar -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib /usr/lib/crt0-efi-x86_64.o {' '.join(e_fl)} -o build/efi/main.efi -lefi -lgnuefi"]).returncode==0 and subprocess.run(["objcopy","-j",".text","-j",".sdata","-j",".data","-j",".dynamic","-j",".dynsym","-j",".rel","-j",".rela","-j",".reloc","--target=efi-app-x86_64","build/efi/main.efi","build/efi/main.efi"]).returncode!=0 or subprocess.run(["strip","-s","build/efi/main.efi"]).returncode!=0):
	quit()
print(f"Linking Kernel: {', '.join([e.replace(chr(92)+'$','$') for e in k_fl])}")
if (subprocess.run(["ld","-melf_x86_64","-o","build/kernel/kernel.elf","-s","-T","kernel.ld","--oformat","elf64-x86-64"]+k_fl).returncode!=0):
	quit()
print(f"Linking LibC: {', '.join([e.replace(chr(92)+'$','$') for e in l_fl])}")
if (subprocess.run(["ld","-melf_x86_64","-o","build/libc/libc.so","-s","-shared","-flinker-output=pie","--oformat","elf64-x86-64"]+l_fl).returncode!=0):
	quit()
print("Creaing OS Image...")
if (subprocess.run(["dd","if=/dev/zero","of=build/os.img","bs=512","count=93750"]).returncode!=0 or subprocess.run(["bash","-c","parted build/os.img -s -a minimal mklabel gpt&&parted build/os.img -s -a minimal mkpart EFI FAT32 2048s 93716s&&parted build/os.img -s -a minimal toggle 1 boot&&dd if=/dev/zero of=build/tmp.img bs=512 count=91669&&mformat -i build/tmp.img -h 32 -t 32 -n 64 -c 1&&mmd -i build/tmp.img ::/EFI ::/EFI/BOOT ::/os&&mcopy -i build/tmp.img build/kernel/kernel.elf ::/KERNEL.ELF&&mcopy -i build/tmp.img build/efi/main.efi ::/EFI/BOOT/BOOTX64.EFI&&mcopy -i build/tmp.img build/libc/libc.so ::/os/libc.so&&dd if=build/tmp.img of=build/os.img bs=512 count=91669 seek=2048 conv=notrunc"]).returncode!=0):
	quit()
dl=[]
for r,sdl,fl in os.walk("build"):
	r=r.replace("\\","/")+"/"
	for f in fl:
		f=r+f
		if (f!="build/os.img"):
			os.remove(f)
	if (r=="build"):
		dl+=sdl
for k in dl:
	os.rmdir(f"build/{k}")
print("Creating Virtual HDD...")
if (subprocess.run(["qemu-img","create","-f","qcow2","build/hdd.qcow2","16G"]).returncode!=0):
	quit()
print("Starting QEMU...")
try:
	subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","max","-smp","cpus=8,sockets=1,cores=8,threads=1","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G"])
except KeyboardInterrupt:
	pass
