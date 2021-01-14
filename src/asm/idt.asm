bits 64
global _asm_setup_idt
global enable_idt
extern idtr



_asm_setup_idt:
	push rax
	
	push rcx
	push rdi
	xor rax, rax
	mov rcx, 0x200
	mov rdi, qword [idtr+2]
	rep stosb
	lidt [idtr]
	pop rdi
	pop rcx
	pop rax
	ret



enable_idt:
	sti
	ret
