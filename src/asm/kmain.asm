%define STACKSIZE 0x4000
bits 64
global asm_move_stack
global asm_end_loop



asm_move_stack:
	pop rax
	mov rbp, (stack+STACKSIZE)
	mov rsp, rbp
	push rax
	ret



asm_end_loop:
	hlt
	jmp asm_end_loop



section .bss
	align 32
	stack:
		resb STACKSIZE
