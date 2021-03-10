bits 64
global asm_current_cpu
global asm_cpu_setup_data
extern asm_apic_setup



asm_cpu_setup_data:
	mov r8, rcx
	mov rcx, __C_MSR_APIC_BASE__
	rdmsr
	shl rdx, 32
	or rax, rdx
	mov rcx, rax
	xor ax, ax
	mov qword [r8 + __C_CPU_STRUCT_APIC_OFFSET__], rax
	push rax
	xor rax, rax
	mov eax, r8d
	mov rcx, __C_MSR_GS_BASE__
	mov rdx, r8
	shr rdx, 32
	wrmsr
	pop rcx
	mov eax, dword [rcx + (__C_APIC_ID_REGISTER__ * __C_SIZEOF_UINT32_T__)]
	shr eax, 24
	mov byte [r8 + __C_CPU_STRUCT_ID_OFFSET__], al
	jmp asm_apic_setup



asm_current_cpu:
	mov rax, qword [gs:__C_CPU_STRUCT_S_OFFSET__]
	ret
