bits 64
global current_cpu



current_cpu:
	mov rax, qword [gs:__C_CPU_STRUCT_S_OFFSET__]
	ret
