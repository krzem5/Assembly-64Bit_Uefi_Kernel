bits 64
global asm_apic_spurious_int
global asm_apic_timer_int
global asm_apic_lint0_int
global asm_apic_lint1_int
global asm_apic_err_int
extern _handle_apic_int



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
