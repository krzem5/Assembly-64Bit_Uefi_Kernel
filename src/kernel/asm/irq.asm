bits 64
global asm_irq0
global asm_irq1
global asm_irq2
global asm_irq3
global asm_irq4
global asm_irq5
global asm_irq6
global asm_irq7
global asm_irq8
global asm_irq9
global asm_irq10
global asm_irq11
global asm_irq12
global asm_irq13
global asm_irq14
global asm_irq15
extern _handle_irq



_asm_irq_h_call:
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
	add rsp, 16
	sti
	iretq



asm_irq0:
	cli
	push byte 0x00
	push byte 0x20
	jmp _asm_irq_h_call



asm_irq1:
	cli
	push byte 0x00
	push byte 0x21
	jmp _asm_irq_h_call



asm_irq2:
	cli
	push byte 0x00
	push byte 0x22
	jmp _asm_irq_h_call



asm_irq3:
	cli
	push byte 0x00
	push byte 0x23
	jmp _asm_irq_h_call



asm_irq4:
	cli
	push byte 0x00
	push byte 0x24
	jmp _asm_irq_h_call



asm_irq5:
	cli
	push byte 0x00
	push byte 0x25
	jmp _asm_irq_h_call



asm_irq6:
	cli
	push byte 0x00
	push byte 0x26
	jmp _asm_irq_h_call



asm_irq7:
	cli
	push byte 0x00
	push byte 0x27
	jmp _asm_irq_h_call



asm_irq8:
	cli
	push byte 0x00
	push byte 0x28
	jmp _asm_irq_h_call



asm_irq9:
	cli
	push byte 0x00
	push byte 0x29
	jmp _asm_irq_h_call



asm_irq10:
	cli
	push byte 0x00
	push byte 0x2a
	jmp _asm_irq_h_call



asm_irq11:
	cli
	push byte 0x00
	push byte 0x2b
	jmp _asm_irq_h_call



asm_irq12:
	cli
	push byte 0x00
	push byte 0x2c
	jmp _asm_irq_h_call



asm_irq13:
	cli
	push byte 0x00
	push byte 0x2d
	jmp _asm_irq_h_call



asm_irq14:
	cli
	push byte 0x00
	push byte 0x2e
	jmp _asm_irq_h_call



asm_irq15:
	cli
	push byte 0x00
	push byte 0x2f
	jmp _asm_irq_h_call
