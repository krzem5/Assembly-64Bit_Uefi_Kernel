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
global asm_isr208
global asm_isr209
global asm_isr210
global asm_isr211
global asm_isr212
global asm_isr213
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
	sti
	iretq



asm_isr0:
	cli
	push byte 0x00
	push byte 0x00
	jmp _asm_isr_h_call



asm_isr1:
	cli
	push byte 0x00
	push byte 0x01
	jmp _asm_isr_h_call



asm_isr2:
	cli
	push byte 0x00
	push byte 0x02
	jmp _asm_isr_h_call



asm_isr3:
	cli
	push byte 0x00
	push byte 0x03
	jmp _asm_isr_h_call



asm_isr4:
	cli
	push byte 0x00
	push byte 0x04
	jmp _asm_isr_h_call



asm_isr5:
	cli
	push byte 0x00
	push byte 0x05
	jmp _asm_isr_h_call



asm_isr6:
	cli
	push byte 0x00
	push byte 0x06
	jmp _asm_isr_h_call



asm_isr7:
	cli
	push byte 0x00
	push byte 0x07
	jmp _asm_isr_h_call



asm_isr8:
	cli
	push byte 0x08
	jmp _asm_isr_h_call



asm_isr9:
	cli
	push byte 0x00
	push byte 0x09
	jmp _asm_isr_h_call



asm_isr10:
	cli
	push byte 0x0a
	jmp _asm_isr_h_call



asm_isr11:
	cli
	push byte 0x0b
	jmp _asm_isr_h_call



asm_isr12:
	cli
	push byte 0x0c
	jmp _asm_isr_h_call



asm_isr13:
	cli
	push byte 0x0d
	jmp _asm_isr_h_call



asm_isr14:
	cli
	push byte 0x0e
	jmp _asm_isr_h_call



asm_isr15:
	cli
	push byte 0x00
	push byte 0x0f
	jmp _asm_isr_h_call



asm_isr16:
	cli
	push byte 0x00
	push byte 0x10
	jmp _asm_isr_h_call



asm_isr17:
	cli
	push byte 0x00
	push byte 0x11
	jmp _asm_isr_h_call



asm_isr18:
	cli
	push byte 0x00
	push byte 0x12
	jmp _asm_isr_h_call



asm_isr19:
	cli
	push byte 0x00
	push byte 0x13
	jmp _asm_isr_h_call



asm_isr20:
	cli
	push byte 0x00
	push byte 0x14
	jmp _asm_isr_h_call



asm_isr21:
	cli
	push byte 0x00
	push byte 0x15
	jmp _asm_isr_h_call



asm_isr22:
	cli
	push byte 0x00
	push byte 0x16
	jmp _asm_isr_h_call



asm_isr23:
	cli
	push byte 0x00
	push byte 0x17
	jmp _asm_isr_h_call



asm_isr24:
	cli
	push byte 0x00
	push byte 0x18
	jmp _asm_isr_h_call



asm_isr25:
	cli
	push byte 0x00
	push byte 0x19
	jmp _asm_isr_h_call



asm_isr26:
	cli
	push byte 0x00
	push byte 0x1a
	jmp _asm_isr_h_call



asm_isr27:
	cli
	push byte 0x00
	push byte 0x1b
	jmp _asm_isr_h_call



asm_isr28:
	cli
	push byte 0x00
	push byte 0x1c
	jmp _asm_isr_h_call



asm_isr29:
	cli
	push byte 0x00
	push byte 0x1d
	jmp _asm_isr_h_call



asm_isr30:
	cli
	push byte 0x00
	push byte 0x1e
	jmp _asm_isr_h_call



asm_isr31:
	cli
	push byte 0x00
	push byte 0x1f
	jmp _asm_isr_h_call



asm_isr208:
	cli
	push byte 0x00
	push word 0xd0
	jmp _asm_isr_h_call



asm_isr209:
	cli
	push byte 0x00
	push word 0xd1
	jmp _asm_isr_h_call



asm_isr210:
	cli
	push byte 0x00
	push word 0xd2
	jmp _asm_isr_h_call



asm_isr211:
	cli
	push byte 0x00
	push word 0xd3
	jmp _asm_isr_h_call



asm_isr212:
	cli
	push byte 0x00
	push word 0xd4
	jmp _asm_isr_h_call



asm_isr213:
	cli
	push byte 0x00
	push word 0xd5
	jmp _asm_isr_h_call
