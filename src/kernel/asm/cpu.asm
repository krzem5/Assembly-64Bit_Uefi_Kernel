bits 64
global current_cpu
global asm_copy_starup_code
extern asm_ap_startup
extern asm_ap_startup.len



current_cpu:
	mov rax, qword [gs:__CPU_STRUCT_S_OFFSET__]
	ret



asm_copy_starup_code:
	push rdi
	push rsi
	mov rdi, rcx
	mov rsi, asm_ap_startup
	mov rcx, asm_ap_startup.len
	shr rcx, 3
	rep movsq
	pop rsi
	pop rdi
	ret
