bits 64
global asm_enable_paging
global asm_halt



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
