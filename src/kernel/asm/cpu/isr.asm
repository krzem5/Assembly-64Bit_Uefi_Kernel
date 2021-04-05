bits 64
global asm_isr0
global asm_isr1
global asm_isr2
global asm_isr3
global asm_isr4
global asm_isr5
global asm_isr6
global asm_isr7
global asm_isr8
global asm_isr9
global asm_isr10
global asm_isr11
global asm_isr12
global asm_isr13
global asm_isr14
global asm_isr15
global asm_isr16
global asm_isr17
global asm_isr18
global asm_isr19
global asm_isr20
global asm_isr21
global asm_isr22
global asm_isr23
global asm_isr24
global asm_isr25
global asm_isr26
global asm_isr27
global asm_isr28
global asm_isr29
global asm_isr30
global asm_isr31
extern _handle_isr



_asm_isr_h_call:
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
	call _handle_isr
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
	iretq



asm_isr0:
	push qword 0x00
	push qword 0x00
	jmp _asm_isr_h_call



asm_isr1:
	push qword 0x00
	push qword 0x01
	jmp _asm_isr_h_call



asm_isr2:
	push qword 0x00
	push qword 0x02
	jmp _asm_isr_h_call



asm_isr3:
	push qword 0x00
	push qword 0x03
	jmp _asm_isr_h_call



asm_isr4:
	push qword 0x00
	push qword 0x04
	jmp _asm_isr_h_call



asm_isr5:
	push qword 0x00
	push qword 0x05
	jmp _asm_isr_h_call



asm_isr6:
	push qword 0x00
	push qword 0x06
	jmp _asm_isr_h_call



asm_isr7:
	push qword 0x00
	push qword 0x07
	jmp _asm_isr_h_call



asm_isr8:
	push qword 0x08
	jmp _asm_isr_h_call



asm_isr9:
	push qword 0x00
	push qword 0x09
	jmp _asm_isr_h_call



asm_isr10:
	push qword 0x0a
	jmp _asm_isr_h_call



asm_isr11:
	push qword 0x0b
	jmp _asm_isr_h_call



asm_isr12:
	push qword 0x0c
	jmp _asm_isr_h_call



asm_isr13:
	push qword 0x0d
	jmp _asm_isr_h_call



asm_isr14:
	push qword 0x0e
	jmp _asm_isr_h_call



asm_isr15:
	push qword 0x00
	push qword 0x0f
	jmp _asm_isr_h_call



asm_isr16:
	push qword 0x00
	push qword 0x10
	jmp _asm_isr_h_call



asm_isr17:
	push qword 0x00
	push qword 0x11
	jmp _asm_isr_h_call



asm_isr18:
	push qword 0x00
	push qword 0x12
	jmp _asm_isr_h_call



asm_isr19:
	push qword 0x00
	push qword 0x13
	jmp _asm_isr_h_call



asm_isr20:
	push qword 0x00
	push qword 0x14
	jmp _asm_isr_h_call



asm_isr21:
	push qword 0x00
	push qword 0x15
	jmp _asm_isr_h_call



asm_isr22:
	push qword 0x00
	push qword 0x16
	jmp _asm_isr_h_call



asm_isr23:
	push qword 0x00
	push qword 0x17
	jmp _asm_isr_h_call



asm_isr24:
	push qword 0x00
	push qword 0x18
	jmp _asm_isr_h_call



asm_isr25:
	push qword 0x00
	push qword 0x19
	jmp _asm_isr_h_call



asm_isr26:
	push qword 0x00
	push qword 0x1a
	jmp _asm_isr_h_call



asm_isr27:
	push qword 0x00
	push qword 0x1b
	jmp _asm_isr_h_call



asm_isr28:
	push qword 0x00
	push qword 0x1c
	jmp _asm_isr_h_call



asm_isr29:
	push qword 0x00
	push qword 0x1d
	jmp _asm_isr_h_call



asm_isr30:
	push qword 0x00
	push qword 0x1e
	jmp _asm_isr_h_call



asm_isr31:
	push qword 0x00
	push qword 0x1f
	jmp _asm_isr_h_call
