bits 64
global __asm_fill_zero



__asm_fill_zero:
	push rdi
	xor rax, rax
	shr rcx, 3
	mov rdi, rdx
	rep stosq
	pop rdi
	ret
