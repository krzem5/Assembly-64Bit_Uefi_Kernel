bits 64
global _asm_irq0
global _asm_irq1
global _asm_irq2
global _asm_irq3
global _asm_irq4
global _asm_irq5
global _asm_irq6
global _asm_irq7
global _asm_irq8
global _asm_irq9
global _asm_irq10
global _asm_irq11
global _asm_irq12
global _asm_irq13
global _asm_irq14
global _asm_irq15
extern _handle_irq



_irq_h_call:
	pushf
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
	call _handle_irq
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
	popf
	add rsp, 16
	iretq



_asm_irq0:
	cli
	push byte 0
	push byte 0x20
	jmp _irq_h_call



_asm_irq1:
	cli
	push byte 0
	push byte 0x21
	jmp _irq_h_call



_asm_irq2:
	cli
	push byte 0
	push byte 0x22
	jmp _irq_h_call



_asm_irq3:
	cli
	push byte 0
	push byte 0x23
	jmp _irq_h_call



_asm_irq4:
	cli
	push byte 0
	push byte 0x24
	jmp _irq_h_call



_asm_irq5:
	cli
	push byte 0
	push byte 0x25
	jmp _irq_h_call



_asm_irq6:
	cli
	push byte 0
	push byte 0x26
	jmp _irq_h_call



_asm_irq7:
	cli
	push byte 0
	push byte 0x27
	jmp _irq_h_call



_asm_irq8:
	cli
	push byte 0
	push byte 0x28
	jmp _irq_h_call



_asm_irq9:
	cli
	push byte 0
	push byte 0x29
	jmp _irq_h_call



_asm_irq10:
	cli
	push byte 0
	push byte 0x2a
	jmp _irq_h_call



_asm_irq11:
	cli
	push byte 0
	push byte 0x2b
	jmp _irq_h_call



_asm_irq12:
	cli
	push byte 0
	push byte 0x2c
	jmp _irq_h_call



_asm_irq13:
	cli
	push byte 0
	push byte 0x2d
	jmp _irq_h_call



_asm_irq14:
	cli
	push byte 0
	push byte 0x2e
	jmp _irq_h_call



_asm_irq15:
	cli
	push byte 0
	push byte 0x2f
	jmp _irq_h_call
