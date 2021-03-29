bits 64
global asm_clear_thread_data



asm_clear_thread_data:
	push rdi
	xor rax, rax
	mov rdi, rcx
	mov rcx, (__C_THREAD_DATA_STRUCT_SIZE__ / __C_SIZEOF_UINT64_T__)
	rep stosq
	pop rdi
	ret
