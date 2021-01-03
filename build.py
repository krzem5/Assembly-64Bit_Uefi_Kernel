import ntpath
import os
import subprocess



if (ntpath.exists("build")):
	os.system("rmdir /s /q build")
os.mkdir("build")



os.mkdir("build\\asm")
os.mkdir("build\\c")
os.mkdir("build\\efi")
a_fl=[]
for k in os.listdir("src\\asm"):
	if (k[-4:]==".asm"):
		if (subprocess.run(["nasm.exe",f"src/asm/{k}","-f","elf64","-Wall","-Werror","-o",f"build/asm/{k[:-4]}.o"]).returncode!=0):
			quit()
		a_fl+=[f"build/asm/{k[:-4]}.o"]
k_fl=[]
e_fl=[]
for r,_,fl in os.walk("src"):
	for f in fl:
		if (r[:7]=="src\\efi"):
			if (f[-2:]==".c"):
				if (subprocess.run(["bash","-c",f"gcc -Isrc/include -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Werror -c {ntpath.join(r,f).replace(chr(92),'/')} -o build/efi/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]
			elif (f[-4:]==".asm"):
				if (subprocess.run(["nasm.exe",ntpath.join(r,f),"-f","elf64","-Wall","-Werror","-o",f"build/efi/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]
		elif (f[-2:]==".c"):
			if (subprocess.run(["bash","-c",f"gcc -Isrc/include -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Werror -c {ntpath.join(r,f).replace(chr(92),'/')} -o build/c/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]).returncode!=0):
				quit()
			k_fl+=[f"build/c/{ntpath.join(r,f)[4:-2].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]
if (subprocess.run(["bash","-c",f"ld -nostdlib -znocombreloc -fshort-wchar -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib /usr/lib/crt0-efi-x86_64.o {' '.join(e_fl)} -o build/efi/main.efi -lefi -lgnuefi"]).returncode==0 and subprocess.run(["objcopy","-j",".text","-j",".sdata","-j",".data","-j",".dynamic","-j",".dynsym","-j",".rel","-j",".rela","-j",".reloc","--target=efi-app-x86_64","build/efi/main.efi","build/efi/main.efi"]).returncode==0 and subprocess.run(["strip","-s","build/efi/main.efi"]).returncode==0 and subprocess.run(["dd","if=/dev/zero","of=build/os.img","bs=512","count=93750"]).returncode==0 and subprocess.run(["bash","-c","parted build/os.img -s -a minimal mklabel gpt&&parted build/os.img -s -a minimal mkpart EFI FAT32 2048s 93716s&&parted build/os.img -s -a minimal toggle 1 boot&&dd if=/dev/zero of=build/tmp.img bs=512 count=91669&&mformat -i build/tmp.img -h 32 -t 32 -n 64 -c 1&&mmd -i build/tmp.img ::/EFI ::/EFI/BOOT&&mcopy -i build/tmp.img build/efi/main.efi ::/EFI/BOOT/BOOTX64.EFI&&dd if=build/tmp.img of=build/os.img bs=512 count=91669 seek=2048 conv=notrunc"]).returncode==0):
	dl=[]
	for r,sdl,fl in os.walk("build"):
		for f in fl:
			f=ntpath.join(r,f)
			if (f!="build\\os.img"):
				os.remove(f)
		if (r=="build"):
			dl+=sdl
	for k in dl:
		os.rmdir(f"build\\{k}")
	if (subprocess.run(["qemu-img","create","-f","qcow2","build\\hdd.qcow2","10G"]).returncode==0):
		os.system("cls")
		subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","qemu64","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G","-net","none"])
