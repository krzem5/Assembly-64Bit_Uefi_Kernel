bits 64
global asm_setup_idt
extern idt



asm_setup_idt:
	push rdi
	xor rax, rax
	mov rcx, 0x200
	mov rdi, qword [idt+2]
	rep stosb
	lidt [idt]
	pop rdi
	ret
