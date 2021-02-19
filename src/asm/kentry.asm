section .entry
	bits 64
	global _start
	extern kmain
	_start:
		cli
		jmp kmain
