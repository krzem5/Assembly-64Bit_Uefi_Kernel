bits 64
global asm_get_cr2



asm_get_cr2:
	mov rax, cr2
	ret
