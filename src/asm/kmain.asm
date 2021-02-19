bits 64
global asm_end_loop
extern kmain_new_stack



asm_end_loop:
	hlt
	jmp asm_end_loop
