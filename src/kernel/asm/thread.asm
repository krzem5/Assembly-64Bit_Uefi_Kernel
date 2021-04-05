bits 64
global asm_clear_thread_data



asm_clear_thread_data:
	push rdi
	xor rax, rax
	mov rdi, rcx
	mov rcx, (__C_THREAD_DATA_STRUCT_SIZE__ / 8)
	rep stosq
	pop rdi
	ret
