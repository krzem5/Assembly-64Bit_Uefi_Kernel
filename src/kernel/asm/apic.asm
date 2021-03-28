bits 64
global asm_apic_setup
global asm_apic_spurious_int
global asm_apic_timer_int
global asm_apic_lint0_int
global asm_apic_lint1_int
global asm_apic_err_int
extern _handle_apic_int
extern hpet_timer_spinwait



%define APIC_TIMER_TARGET_US 10



_asm_isr_h_call:
	pushfq
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	mov rcx, rsp
	call _handle_apic_int
	mov rax, qword [gs:__C_CPU_STRUCT_APIC_OFFSET__]
	mov dword [rax + (__C_APIC_EOI_REGISTER__ * __C_SIZEOF_UINT32_T__)], 0
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	popfq
	add rsp, 8
	iretq



section .unmap
asm_apic_setup:
	cmp dword [rcx + (__C_APIC_LVT_TIMER_REGISER__ * __C_SIZEOF_UINT32_T__)], (__C_APIC_SPURIOUS_INTERRUPT__ | __C_APIC_SVR_ENABLE__)
	je ._end
	mov dword [rcx + (__C_APIC_LVT_ERROR_REGISER__ * __C_SIZEOF_UINT32_T__)], __C_APIC_ERROR_INTERRUPT__
	mov dword [rcx + (__C_APIC_SPURIOUS_REGISTER__ * __C_SIZEOF_UINT32_T__)], (__C_APIC_SPURIOUS_INTERRUPT__ | __C_APIC_SVR_ENABLE__)
	mov dword [rcx + (__C_APIC_LVT_TIMER_REGISER__ * __C_SIZEOF_UINT32_T__)], __C_APIC_SPURIOUS_INTERRUPT__
	mov dword [rcx + (__C_APIC_TIMER_DIVISOR_REGISER__ * __C_SIZEOF_UINT32_T__)], 0xb
	xor rdx, rdx
	dec edx
	mov dword [rcx + (__C_APIC_TIMER_INIT_REGISER__ * __C_SIZEOF_UINT32_T__)], edx
	push rdx
	push rcx
	mov rcx, __C_APIC_TIMER_CALIB_US__
	call hpet_timer_spinwait
	pop r8
	pop rdx
	xor rax, rax
	mov eax, dword [r8 + (__C_APIC_TIMER_VALUE_REGISER__ * __C_SIZEOF_UINT32_T__)]
	sub edx, eax
	mov eax, edx
	xor rdx, rdx
	div rcx
	mov rcx, qword [gs:__C_CPU_STRUCT_S_OFFSET__]
	mov dword [rcx + __C_CPU_STRUCT_APIC_TPUS_OFFSET__], eax
	mov dword [r8 + (__C_APIC_LVT_TIMER_REGISER__ * __C_SIZEOF_UINT32_T__)], (__C_APIC_TIMER_REPEAT__ | __C_APIC_TIMER_INTERRUPT__)
	mov rdx, APIC_TIMER_TARGET_US
	mul rdx
	mov dword [r8 + (__C_APIC_TIMER_INIT_REGISER__ * __C_SIZEOF_UINT32_T__)], eax
._end:
	ret



section .text
asm_apic_spurious_int:
	iretq



asm_apic_timer_int:
	push qword 0xd1
	jmp _asm_isr_h_call



asm_apic_lint0_int:
	push qword 0xd2
	jmp _asm_isr_h_call



asm_apic_lint1_int:
	push qword 0xd3
	jmp _asm_isr_h_call



asm_apic_err_int:
	push qword 0xd4
	jmp _asm_isr_h_call
