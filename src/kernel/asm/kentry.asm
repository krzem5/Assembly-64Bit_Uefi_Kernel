section .entry
	bits 64
	global _start
	extern kmain
	_start:
		cli
		mov rbp, qword [rcx + __KERNEL_ARGS_STRUCT_STACK_POINTER_OFFSET__]
		mov rsp, rbp
		jmp kmain
