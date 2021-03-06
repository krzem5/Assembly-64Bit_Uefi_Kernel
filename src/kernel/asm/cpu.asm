bits 64
global current_cpu



current_cpu:
	mov rax, [gs:__CPU_STRUCT_S_OFFSET__]
	ret
