bits 64
global _asm_isr0
global _asm_isr1
global _asm_isr2
global _asm_isr3
global _asm_isr4
global _asm_isr5
global _asm_isr6
global _asm_isr7
global _asm_isr8
global _asm_isr9
global _asm_isr10
global _asm_isr11
global _asm_isr12
global _asm_isr13
global _asm_isr14
global _asm_isr15
global _asm_isr16
global _asm_isr17
global _asm_isr18
global _asm_isr19
global _asm_isr20
global _asm_isr21
global _asm_isr22
global _asm_isr23
global _asm_isr24
global _asm_isr25
global _asm_isr26
global _asm_isr27
global _asm_isr28
global _asm_isr29
global _asm_isr30
global _asm_isr31
extern _handle_isr



_isr_h_call:
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
	pop rax
	ret



_asm_isr0:
	cli
	push byte 0
	push byte 0x00
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr1:
	cli
	push byte 0
	push byte 0x01
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr2:
	cli
	push byte 0
	push byte 0x02
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr3:
	cli
	push byte 0
	push byte 0x03
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr4:
	cli
	push byte 0
	push byte 0x04
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr5:
	cli
	push byte 0
	push byte 0x05
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr6:
	cli
	push byte 0
	push byte 0x06
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr7:
	cli
	push byte 0
	push byte 0x07
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr8:
	cli
	push byte 0x08
	call _isr_h_call
	add rsp, 16
	iretq


_asm_isr9:
	cli
	push byte 0
	push byte 0x0
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr10:
	cli
	push byte 0x0a
	call _isr_h_call
	add rsp, 8
	iretq


_asm_isr11:
	cli
	push byte 0x0b
	call _isr_h_call
	add rsp, 8
	iretq


_asm_isr12:
	cli
	push byte 0x0c
	call _isr_h_call
	add rsp, 8
	iretq


_asm_isr13:
	cli
	push byte 0x0d
	call _isr_h_call
	add rsp, 8
	iretq


_asm_isr14:
	cli
	push byte 0x0e
	call _isr_h_call
	add rsp, 8
	iretq


_asm_isr15:
	cli
	push byte 0
	push byte 0x0f
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr16:
	cli
	push byte 0
	push byte 0x10
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr17:
	cli
	push byte 0
	push byte 0x11
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr18:
	cli
	push byte 0
	push byte 0x12
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr19:
	cli
	push byte 0
	push byte 0x13
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr20:
	cli
	push byte 0
	push byte 0x14
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr21:
	cli
	push byte 0
	push byte 0x15
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr22:
	cli
	push byte 0
	push byte 0x16
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr23:
	cli
	push byte 0
	push byte 0x17
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr24:
	cli
	push byte 0
	push byte 0x18
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr25:
	cli
	push byte 0
	push byte 0x19
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr26:
	cli
	push byte 0
	push byte 0x1a
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr27:
	cli
	push byte 0
	push byte 0x1b
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr28:
	cli
	push byte 0
	push byte 0x1c
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr29:
	cli
	push byte 0
	push byte 0x1d
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr30:
	cli
	push byte 0
	push byte 0x1e
	call _isr_h_call
	add rsp, 16
	iretq



_asm_isr31:
	cli
	push byte 0
	push byte 0x1f
	call _isr_h_call
	add rsp, 16
	iretq
