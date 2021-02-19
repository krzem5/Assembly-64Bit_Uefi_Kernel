bits 64
global asm_move_stack
global asm_end_loop
extern kmain_new_stack



asm_move_stack:
	mov rbp, rcx
	mov rsp, rbp
	mov rcx, rdx
	jmp kmain_new_stack



asm_end_loop:
	hlt
	jmp asm_end_loop
