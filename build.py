import hashlib
import os
import subprocess
import sys
import re
import requests



FILE_HASH_BUF_SIZE=65536
FONT_OUT_NAME="font_spleen"
FONT_URL="https://raw.githubusercontent.com/fcambus/spleen/master/spleen-8x16.bdf"
FONT_MAX_CHAR=0x7e
INCLUDE_LIST_REGEX=re.compile(br"^\s*?((?:\s*#\s*include\s*<[^>]*?>)+)",re.M)
INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$")
REQUIRED_STRUCTURE_OFFSETS={b"__KERNEL_ARGS":[b"k_sp"],b"__CPU":[b"s",b"rsp0"],b"__CPUID_INFO":[b"eax",b"ebx",b"ecx",b"edx"],b"__IDT_TABLE":[b"b"]}
REQUIRED_STRUCTURE_SIZE=[b"__THREAD_DATA",b"__IDT_ENTRY"]
REQUIRED_DEFINITIONS=[b"LOW_MEM_AP_INIT_ADDR",b"LOW_MEM_AP_PML4_ADDR",b"MSR_GS_BASE",b"PAGE_PRESENT",b"PAGE_READ_WRITE",b"PAGE_DIR_PRESENT",b"PAGE_DIR_READ_WRITE",b"PAGE_4KB_SIZE",b"TOTAL_INTERRUPT_NUMBER"]
SIZEOF_POINTER=8
SIZEOF_UINT8_T=1
SIZEOF_UINT16_T=2
SIZEOF_UINT32_T=4
SIZEOF_UINT64_T=8



def _sort_inc(m,il):
	l=[INCLUDE_FILE_REGEX.search(e.strip()).group(1) for e in m.group(1).strip().split(b"\n")]
	il.extend(l)
	return (b"#include <shared.h>"+(b"\n" if len(l)>1 else b"") if b"shared.h" in l else b"")+b"\n".join([b"#include <"+e+b">" for e in sorted(l) if e!=b"shared.h"])



def _check_new(f_h_dt,n_f_h_dt,f_inc,fp):
	if (fp not in f_h_dt or f_h_dt[fp]!=n_f_h_dt[fp]):
		return True
	if (fp not in f_inc):
		return False
	for k in f_inc[fp]:
		if (k not in f_h_dt or f_h_dt[k]!=n_f_h_dt[k]):
			return True
	return False



if (not os.path.exists("build")):
	os.mkdir("build")
f_h_dt={}
if (os.path.exists("build/__last_build_files__")):
	with open("build/__last_build_files__","r") as f:
		for k in f.read().split("\n"):
			k=k.strip()
			if (len(k)<33):
				continue
			f_h_dt[k[:-32]]=k[-32:]
for k in os.listdir("build"):
	if (os.path.isfile(f"build/{k}")):
		os.remove(f"build/{k}")
if (not os.path.exists("build/efi")):
	os.mkdir("build/efi")
if (not os.path.exists("build/kernel")):
	os.mkdir("build/kernel")
if (not os.path.exists("build/libc")):
	os.mkdir("build/libc")
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
					o[id_*2+j//8]|=int(bin(int(dt[i],16))[2:].rjust(8,"0")[::-1],2)<<(8*(j%8))
					j+=1
				i+=1
		i+=1
	with open(f"rsrc/include/{FONT_OUT_NAME}.h","w") as f:
		f.write(f"#ifndef __{FONT_OUT_NAME.upper()}_H__\n#define __{FONT_OUT_NAME.upper()}_H__ 1\n#include <gfx/font.h>\n#include <stdint.h>\n\n\n\nextern const uint64_t {FONT_OUT_NAME.upper()}_DATA[];\n\n\n\nextern Font {FONT_OUT_NAME.upper()};\n\n\n\n#endif\n")
	with open(f"rsrc/{FONT_OUT_NAME}.c","w") as f:
		f.write(f"#include <gfx/font.h>\n#include <stdint.h>\n#include <{FONT_OUT_NAME}.h>\n\n\n\nconst uint64_t {FONT_OUT_NAME.upper()}_DATA[{(FONT_MAX_CHAR+1)*2}]={{\n\t{(','+chr(10)+chr(9)).join(['0x'+hex(e)[2:].rjust(16,'0') for e in o])}\n}};\n\n\n\nFont {FONT_OUT_NAME.upper()}={{\n\t{FONT_MAX_CHAR},\n\t{FONT_OUT_NAME.upper()}_DATA\n}};\n")
src_fl=list(os.walk("src"))+list(os.walk("rsrc"))
asm_d=[]
fd=[]
f_inc={}
n_f_h_dt={}
asm_d_fl={}
for k,v in REQUIRED_STRUCTURE_OFFSETS.items():
	for e in v:
		asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(e,'utf-8').upper()}_OFFSET__"]=[[],[]]
for k in REQUIRED_STRUCTURE_SIZE:
	asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__"]=[[],[]]
for k in REQUIRED_DEFINITIONS:
	asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}__"]=[[],[]]
for r,_,fl in src_fl:
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (f[-2:]==".c" or f[-2:]==".h"):
			print(f"Prettifying C {('Source' if f[-1]=='c' else 'Header')} File: {r+f}")
			with open(r+f,"rb") as rf:
				il=[]
				dt=INCLUDE_LIST_REGEX.sub(lambda m:_sort_inc(m,il),rf.read())
				f_inc[r+f]=[]
				for e in il:
					e=str(e.replace(b"\\",b"/").split(b"/")[-1],"utf-8")
					for tr,_,tfl in src_fl:
						tr=tr.replace("\\","/")+"/"
						for tf in tfl:
							if ((tr+tf).replace("\\","/").split("/")[-1]==e):
								f_inc[r+f]+=[(tr+tf)]
			h=hashlib.md5()
			h.update(dt)
			n_f_h_dt[r+f]=h.hexdigest()
			if (f[-1]=="h"):
				for k in set(REQUIRED_STRUCTURE_OFFSETS.keys())|set(REQUIRED_STRUCTURE_SIZE):
					m=re.search(br"struct\s+"+k+br"\s*\{",dt)
					if (m is not None):
						i=m.end()
						b=1
						while (b>0 or dt[i:i+1] in b" \t\r\n\v\f"):
							if (dt[i:i+1]==b"{"):
								b+=1
							elif (dt[i:i+1]==b"}"):
								b-=1
							i+=1
						p=False
						if (dt[i:i+15]==b"__attribute__((" and dt[i+15:].split(b")")[0].strip(b"_")==b"packed"):
							p=True
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
							t=e[:-len(e.split(b" ")[-1])].strip()
							c=0
							if (t[:1]==b"}"):
								break
							elif (t[-1:]==b"*"):
								c=SIZEOF_POINTER
							elif (t==b"uint8_t"):
								c=SIZEOF_UINT8_T
							elif (t==b"uint16_t"):
								c=SIZEOF_UINT16_T
							elif (t==b"uint32_t"):
								c=SIZEOF_UINT32_T
							elif (t==b"uint64_t"):
								c=SIZEOF_UINT64_T
							else:
								print(f"Unknown sizeof of Type '{str(t,'utf-8')}'!")
								quit()
							if (p==False and lc!=-1 and lc<c and off%c!=0):
								off+=c-off%c
							if (k in REQUIRED_STRUCTURE_OFFSETS):
								nm=e.split(b" ")[-1].replace(b"[]",b"")
								if (nm in REQUIRED_STRUCTURE_OFFSETS[k]):
									asm_d+=[f"-D__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(nm,'utf-8').upper()}_OFFSET__={off}"]
									asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(nm,'utf-8').upper()}_OFFSET__"][0]+=[r+f]
									REQUIRED_STRUCTURE_OFFSETS[k].remove(nm)
									if (len(REQUIRED_STRUCTURE_OFFSETS[k])==0 and k not in REQUIRED_STRUCTURE_SIZE):
										break
							off+=c
							lc=c
						if (k in REQUIRED_STRUCTURE_SIZE):
							asm_d+=[f"-D__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__={off}"]
							asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__"][0]+=[r+f]
							REQUIRED_STRUCTURE_SIZE.remove(k)
						if (k in REQUIRED_STRUCTURE_OFFSETS):
							if (REQUIRED_STRUCTURE_OFFSETS[k]):
								for e in REQUIRED_STRUCTURE_OFFSETS[k]:
									print(f"Unable to Find Element '{str(e,'utf-8')}' in Structure '{str(k,'utf-8')}'!")
								quit()
							del REQUIRED_STRUCTURE_OFFSETS[k]
			for k in REQUIRED_DEFINITIONS:
				l=re.findall(br"\#define\s+"+k+br"\s*(.*)$",dt,re.M)
				if (len(l)==0):
					continue
				if (k not in fd and len(l)==1):
					asm_d+=[f"-D__C_{str(k,'utf-8').strip('_').upper()}__={str(l[0].strip(),'utf-8')}"]
					asm_d_fl[f"__C_{str(k,'utf-8').strip('_').upper()}__"][0]+=[r+f]
					fd+=[k]
				else:
					print(f"Found Multiple Preprocessor Definitions of '{str(k,'utf-8')}!")
					quit()
			with open(r+f,"wb") as wf:
				wf.write(dt)
		elif (f[-4:]==".asm"):
			dt=b""
			with open(r+f,"rb") as rf:
				h=hashlib.md5()
				while (True):
					c=rf.read(FILE_HASH_BUF_SIZE)
					dt+=c
					if (len(c)==0):
						break
					h.update(c)
				n_f_h_dt[r+f]=h.hexdigest()
			dt=str(dt,"utf-8")
			for k in asm_d_fl:
				if (k in dt):
					asm_d_fl[k][1]+=[r+f]
			f_inc[r+f]=[]
for k in REQUIRED_STRUCTURE_OFFSETS.keys():
	print(f"Unable to Find Struture '{str(k,'utf-8')}'!")
for k in REQUIRED_STRUCTURE_SIZE:
	print(f"Unable to Find Struture '{str(k,'utf-8')}'!")
err=False
for k,v in asm_d_fl.items():
	if (len(v[1])==0):
		print(f"External Definition '{k}' not used in Assembly!")
		err=True
	else:
		for e in v[1]:
			for se in v[0]:
				if (se not in f_inc[e]):
					f_inc[e]+=[se]
if (REQUIRED_STRUCTURE_OFFSETS or REQUIRED_STRUCTURE_SIZE or err):
	quit()
while (True):
	u=False
	for k,v in f_inc.items():
		for e in v:
			for se in f_inc[e]:
				if (se not in v):
					u=True
					v.append(se)
	if (not u):
		break
e_fl=[]
k_fl=[]
l_fl=[]
cl={k:(v if k[-2:]!=".h" else n_f_h_dt[k]) for k,v in f_h_dt.items() if k in n_f_h_dt}
for r,_,fl in src_fl:
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (r[:7]=="src/efi"):
			if (f[-2:]==".c"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling C File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o")
					if (subprocess.run(["bash","-c",f"gcc -Isrc/kernel/include -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -fno-stack-protector -O3 -fpic -fshort-wchar -fno-common -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Werror -c {r+f} -o build/efi/{(r+f)[8:].replace('/',chr(92)+'$')}.o"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/efi/{(r+f)[8:].replace('/',chr(92)+'$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling C File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o (Already Exists)")
				e_fl+=[f"build/efi/{(r+f)[8:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
			elif (f[-4:]==".asm"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling ASM File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o")
					if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/efi/{(r+f)[8:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling ASM File (Efi): {r+f} -> build/efi/{(r+f)[8:].replace('/','$')}.o (Already Exists)")
				e_fl+=[f"build/efi/{(r+f)[8:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
		elif (r[:10]=="src/kernel"):
			if (f[-2:]==".c"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling C File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o","-Isrc/kernel/include","-Irsrc/include","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling C File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
			elif (f[-4:]==".asm"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling ASM File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]+asm_d).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling ASM File (Kernel): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
		elif (r[:4]=="rsrc"):
			if (f[-2:]==".c"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling C File (Kernel Resource): {r+f} -> build/kernel/{(r+f).replace('/','$')}.o")
					if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f).replace('/','$')}.o","-Irsrc/include","-Isrc/kernel/include","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f).replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling C File (Kernel Resource): {r+f} -> build/kernel/{(r+f).replace('/','$')}.o (Already Exists)")
				k_fl+=[f"build/kernel/{(r+f).replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
		else:
			if (f[-2:]==".c"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling C File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o","-Isrc/kernel/include","-Isrc/libc/include","-Irsrc","-D__KERNEL__=1"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
					print(f"Compiling C File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["gcc","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",r+f,"-o",f"build/libc/{(r+f)[4:].replace('/','$')}.o","-Isrc/libc/include","-Irsrc"]).returncode!=0 or subprocess.run(["strip","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/libc/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling C File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
					print(f"Compiling C File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				l_fl+=[f"build/libc/{(r+f)[4:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
			elif (f[-4:]==".asm"):
				if (_check_new(f_h_dt,n_f_h_dt,f_inc,r+f)):
					print(f"Compiling ASM File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-D__KERNEL__=1","-o",f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
					print(f"Compiling ASM File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o")
					if (subprocess.run(["nasm",r+f,"-f","elf64","-O3","-Wall","-Werror","-o",f"build/libc/{(r+f)[4:].replace('/','$')}.o"]).returncode!=0):
						print("Writing File Hash List...")
						with open("build/__last_build_files__","w") as f:
							for k,v in cl.items():
								f.write(f"{k}{v}\n")
						quit()
				else:
					print(f"Compiling ASM File (Kernel LibC): {r+f} -> build/kernel/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
					print(f"Compiling ASM File (LibC): {r+f} -> build/libc/{(r+f)[4:].replace('/','$')}.o (Already Exists)")
				k_fl+=[f"build/kernel/{(r+f)[4:].replace('/','$')}.o"]
				l_fl+=[f"build/libc/{(r+f)[4:].replace('/','$')}.o"]
				cl[r+f]=n_f_h_dt[r+f]
print("Writing File Hash List...")
with open("build/__last_build_files__","w") as f:
	for k,v in n_f_h_dt.items():
		f.write(f"{k}{v}\n")
print(f"Linking EFI OS Loader: {', '.join(e_fl)}")
if (subprocess.run(["bash","-c",f"ld -nostdlib -znocombreloc -fshort-wchar -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib /usr/lib/crt0-efi-x86_64.o {' '.join(e_fl)} -o build/loader.efi -lefi -lgnuefi"]).returncode==0 and subprocess.run(["objcopy","-j",".text","-j",".sdata","-j",".data","-j",".dynamic","-j",".dynsym","-j",".rel","-j",".rela","-j",".reloc","--target=efi-app-x86_64","build/loader.efi","build/loader.efi"]).returncode!=0 or subprocess.run(["strip","-s","build/loader.efi"]).returncode!=0):
	quit()
print(f"Linking Kernel: {', '.join(k_fl)}")
if (subprocess.run(["ld","-melf_x86_64","-o","build/kernel.elf","-s","-T","kernel.ld","--oformat","elf64-x86-64"]+k_fl).returncode!=0):
	quit()
print(f"Linking Debug Kernel: {', '.join(k_fl)}")
if (subprocess.run(["ld","-melf_x86_64","-o","build/dbg_kernel.elf","-T","kernel.ld","--oformat","elf64-x86-64"]+k_fl).returncode!=0):
	quit()
print(f"Linking LibC: {', '.join(l_fl)}")
if (subprocess.run(["ld","-melf_x86_64","-o","build/libc.so","-s","-shared","-flinker-output=pie","--oformat","elf64-x86-64"]+l_fl).returncode!=0):
	quit()
print("Creaing OS Image...")
if (subprocess.run(["dd","if=/dev/zero","of=build/os.img","bs=512","count=93750"]).returncode!=0 or subprocess.run(["bash","-c","parted build/os.img -s -a minimal mklabel gpt&&parted build/os.img -s -a minimal mkpart EFI FAT32 2048s 93716s&&parted build/os.img -s -a minimal toggle 1 boot&&dd if=/dev/zero of=build/tmp.img bs=512 count=91669&&mformat -i build/tmp.img -h 32 -t 32 -n 64 -c 1&&mmd -i build/tmp.img ::/EFI ::/EFI/BOOT ::/os&&mcopy -i build/tmp.img build/kernel.elf ::/KERNEL.ELF&&mcopy -i build/tmp.img build/loader.efi ::/EFI/BOOT/BOOTX64.EFI&&mcopy -i build/tmp.img build/libc.so ::/os/libc.so&&dd if=build/tmp.img of=build/os.img bs=512 count=91669 seek=2048 conv=notrunc"]).returncode!=0):
	quit()
os.remove("build/loader.efi")
os.remove("build/kernel.elf")
os.remove("build/libc.so")
os.remove("build/tmp.img")
print("Creating Virtual HDD...")
if (subprocess.run(["qemu-img","create","-f","qcow2","build/hdd.qcow2","16G"]).returncode!=0):
	quit()
print("Starting QEMU...")
try:
	subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","max","-smp","cpus=8,sockets=1,cores=8,threads=1","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G"])
except KeyboardInterrupt:
	pass
