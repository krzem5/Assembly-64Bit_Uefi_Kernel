bits 64
global asm_setup_idt
extern idtr



asm_setup_idt:
	push rdi
	xor rax, rax
	mov rcx, 0x200
	mov rdi, qword [idtr+2]
	rep stosb
	lidt [idtr]
	pop rdi
	ret
