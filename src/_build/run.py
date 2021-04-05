import subprocess



def run(dbg):
	print("Creating Virtual HDD...")
	if (subprocess.run(["qemu-img","create","-f","qcow2","build/hdd.qcow2","16G"]).returncode!=0):
		quit()
	print("Starting QEMU...")
	try:
		subprocess.run(["qemu-system-x86_64","-bios","OVMF.fd","-cpu","max","-smp","cpus=8,sockets=1,cores=8,threads=1","-hda","build/hdd.qcow2","-boot","d","-drive","file=build/os.img,if=ide,format=raw","-m","4G"])
	except KeyboardInterrupt:
		pass
