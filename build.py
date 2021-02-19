import ntpath
import os
import subprocess



if (ntpath.exists("build")):
	os.system("rmdir /s /q build")
os.mkdir("build")



os.mkdir("build\\kernel")
os.mkdir("build\\efi")
k_fl=[]
e_fl=[]
for r,_,fl in os.walk("src"):
	for f in fl:
		if (r[:7]=="src\\efi"):
			if (f[-2:]==".c"):
				print(f"Compiling EFI-C File: {ntpath.join(r,f).replace(chr(92),'/')} -> build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/','$')}.o")
				if (subprocess.run(["bash.exe","-c",f"gcc -Isrc/include -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -fno-stack-protector -O3 -fpic -fshort-wchar -fno-common -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Werror -c {ntpath.join(r,f).replace(chr(92),'/')} -o build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]).returncode!=0 or subprocess.run(["strip.exe","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/',chr(92)+'$')}.o"]
			elif (f[-4:]==".asm"):
				print(f"Compiling EFI-ASM File: {ntpath.join(r,f).replace(chr(92),'/')} -> build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/','$')}.o")
				if (subprocess.run(["nasm.exe",ntpath.join(r,f),"-f","elf64","-O3","-Wall","-Werror","-o",f"build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/','$')}.o"]).returncode!=0):
					quit()
				e_fl+=[f"build/efi/{ntpath.join(r,f)[8:].replace(chr(92),'/').replace('/','$')}.o"]
		elif (f[-2:]==".c"):
			print(f"Compiling C File: {ntpath.join(r,f).replace(chr(92),'/')} -> build/kernel/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o")
			if (subprocess.run(["gcc.exe","-mcmodel=large","-mno-red-zone","-fno-common","-m64","-Wall","-Werror","-fpic","-ffreestanding","-fno-stack-protector","-O3","-nostdinc","-nostdlib","-c",ntpath.join(r,f).replace(chr(92),'/'),"-o",f"build\\kernel\\{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o","-Isrc\\include","-Irsrc"]).returncode!=0 or subprocess.run(["strip.exe","-R",".rdata$zzz","--keep-file-symbols","--strip-debug","--strip-unneeded","--discard-locals",f"build\\kernel\\{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]).returncode!=0):
				quit()
			k_fl+=[f"build/kernel/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]
		elif (f[-4:]==".asm"):
			print(f"Compiling ASM File: {ntpath.join(r,f).replace(chr(92),'/')} -> build/kernel/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o")
			if (subprocess.run(["nasm.exe",ntpath.join(r,f).replace(chr(92),'/'),"-f","elf64","-O3","-Wall","-Werror","-o",f"build/kernel/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]).returncode!=0):
				quit()
			k_fl+=[f"build/kernel/{ntpath.join(r,f)[4:].replace(chr(92),'/').replace('/','$')}.o"]
print(f"Linking EFI OS Loader: {', '.join([e.replace(chr(92)+'$','$') for e in e_fl])}")
if (subprocess.run(["bash.exe","-c",f"ld -nostdlib -znocombreloc -fshort-wchar -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib /usr/lib/crt0-efi-x86_64.o {' '.join(e_fl)} -o build/efi/main.efi -lefi -lgnuefi"]).returncode==0 and subprocess.run(["objcopy","-j",".text","-j",".sdata","-j",".data","-j",".dynamic","-j",".dynsym","-j",".rel","-j",".rela","-j",".reloc","--target=efi-app-x86_64","build/efi/main.efi","build/efi/main.efi"]).returncode==0 and subprocess.run(["strip","-s","build/efi/main.efi"]).returncode==0):
	print(f"Linking Kernel: {', '.join([e.replace(chr(92)+'$','$') for e in k_fl])}")
	if (subprocess.run(["C:\\ProgramFiles\\Cygwin\\bin\\ld.exe","-melf_x86_64","-o","build\\kernel\\kernel.elf","-T","linker.ld","--oformat","elf64-x86-64"]+k_fl).returncode==0):
		print("Creaing OS Image...")
		if (subprocess.run(["dd","if=/dev/zero","of=build/os.img","bs=512","count=93750"]).returncode==0 and subprocess.run(["bash","-c","parted build/os.img -s -a minimal mklabel gpt&&parted build/os.img -s -a minimal mkpart EFI FAT32 2048s 93716s&&parted build/os.img -s -a minimal toggle 1 boot&&dd if=/dev/zero of=build/tmp.img bs=512 count=91669&&mformat -i build/tmp.img -h 32 -t 32 -n 64 -c 1&&mmd -i build/tmp.img ::/EFI ::/EFI/BOOT&&mcopy -i build/tmp.img build/kernel/kernel.elf ::/KERNEL.ELF&&mcopy -i build/tmp.img build/efi/main.efi ::/EFI/BOOT/BOOTX64.EFI&&dd if=build/tmp.img of=build/os.img bs=512 count=91669 seek=2048 conv=notrunc"]).returncode==0):
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
			print("Creating Virtual HDD...")
			if (subprocess.run(["qemu-img","create","-f","qcow2","build\\hdd.qcow2","16G"]).returncode==0):
				print("Starting QEMU...")
				os.system("cls")
				subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","max","-smp","cpus=8,sockets=2,cores=4,threads=1","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G"])
