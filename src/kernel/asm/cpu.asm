bits 64
global asm_current_cpu



asm_current_cpu:
	mov rax, qword [gs:__C_CPU_STRUCT_S_OFFSET__]
	ret
