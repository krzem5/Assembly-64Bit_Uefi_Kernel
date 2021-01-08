%define STACKSIZE 0x4000
section .entry
	bits 64
	global _start
	extern kmain
	_start:
		cli
		; mov rbp, (stack+STACKSIZE)
		; mov rsp, rbp
		call kmain
		._s_loop:
			hlt
			jmp ._s_loop
section .bss
	align 32
	stack:
		resb STACKSIZE
