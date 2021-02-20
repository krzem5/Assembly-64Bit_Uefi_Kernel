import ntpath
import os
import subprocess
import re



INCLUDE_LIST_REGEX=re.compile(br"^\s*?((?:\s*#\s*include\s*<[^>]*?>)+)",re.M)
INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$")
KERNEL_ARGS_STRUCT_HEADER="src/kernel/include/kmain.h"
KERNEL_ARGS_STRUCT_REGEX=re.compile(br"struct\s+__KERNEL_ARGS\s*{")
KERNEL_ARGS_STRUCT_STACK_POINTER_NAME=b"k_sp"
SIZEOF_64BIT_POINTER=8
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
for r,_,fl in os.walk("src"):
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (f[-2:]==".c" or f[-2:]==".h"):
			print(f"Prettifying C {('Source' if f[-1]=='c' else 'Header')} File: {r+f}")
			with open(r+f,"rb") as rf:
				dt=INCLUDE_LIST_REGEX.sub(_sort_inc,rf.read())
			with open(r+f,"wb") as wf:
				wf.write(dt)
for r,_,fl in os.walk("src"):
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
				if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o","-Isrc/kernel/include","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
			elif (f[-4:]==".asm"):
				print(f"Compiling ASM File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
				ef=[]
				if (f=="kentry.asm"):
					with open(KERNEL_ARGS_STRUCT_HEADER,"rb") as hf:
						dt=hf.read()
						m=KERNEL_ARGS_STRUCT_REGEX.search(dt)
						if (m is None):
							print(f"No __KERNEL_ARGS struct found in file '{KERNEL_ARGS_STRUCT_HEADER}'!")
							quit()
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
							nm=e.split(b" ")[-1].replace(b"[]",b"")
							if (nm==KERNEL_ARGS_STRUCT_STACK_POINTER_NAME):
								break
							e=e[:-len(e.split(b" ")[-1])].strip()
							c=0
							if (e[-1:]==b"*"):
								c=SIZEOF_64BIT_POINTER
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
						ef=[f"-D__KERNEL_ARGS_STRUCT_STACK_POINTER_OFFSET__={off}"]
				if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]+ef).returncode!=0):
					quit()
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
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
	subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","max","-smp","cpus=8,sockets=2,cores=4,threads=1","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G"])
except KeyboardInterrupt:
	pass
