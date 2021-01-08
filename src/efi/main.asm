bits 64
global asm_clear_pages_get_cr3
global asm_enable_paging
global asm_halt



asm_clear_pages_get_cr3:
	mov rdi, rcx
	xor rax, rax
	mov rcx, rdx
	shl rcx, 9
	rep stosq
	mov rax, cr3
	ret



asm_enable_paging:
	mov rax, cr4
	or rax, 0xa0
	mov cr4, rax
	mov cr3, rcx
	mov rax, cr0
	bts rax, 31
	mov cr0, rax
	ret



asm_halt:
	cli
._loop:
	hlt
	jmp ._loop
