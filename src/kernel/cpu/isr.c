#include <shared.h>
#include <cpu/fatal_error.h>
#include <cpu/idt.h>
#include <cpu/isr.h>
#include <gfx/console.h>
#include <stddef.h>
#include <stdint.h>



const char* _f_msg[32]={
	"Divide-by-zero Error",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Security Exception",
	"Reserved"
};
isr_handler_t isr_hl[32]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};



void KERNEL_CALL _handle_isr(registers_t* r){
	if (isr_hl[r->t]&&isr_hl[r->t](r)){
		return;
	}
	uint64_t cr0;
	uint64_t cr2;
	uint64_t cr3;
	uint64_t cr4;
	uint64_t cr8;
	__asm__ volatile("mov %%cr0, %%rax\nmov %%rax, %0\nmov %%cr2, %%rax\nmov %%rax, %1\nmov %%cr3, %%rax\nmov %%rax, %2\nmov %%cr4, %%rax\nmov %%rax, %3\nmov %%cr8, %%rax\nmov %%rax, %4":"=m"(cr0),"=m"(cr2),"=m"(cr3),"=m"(cr4),"=m"(cr8)::"%rax");
	console_error("Error:\n  rax    = %#.18llx\n  rbx    = %#.18llx\n  rcx    = %#.18llx\n  rdx    = %#.18llx\n  rsi    = %#.18llx\n  rdi    = %#.18llx\n  rbp    = %#.18llx\n  r8     = %#.18llx\n  r9     = %#.18llx\n  r10    = %#.18llx\n  r11    = %#.18llx\n  r12    = %#.18llx\n  r13    = %#.18llx\n  r14    = %#.18llx\n  r15    = %#.18llx\n  t      = %#.4llx (%s)\n  ec     = %#.4llx\n  rip    = %#.18llx\n  cs     = %#.6llx\n  rflags = %#.8llx\n  rsp    = %#.18llx\n  ss     = %#.6llx\n  cr0    = %#.18llx\n  cr2    = %#.18llx\n  cr3    = %#.18llx\n  cr4    = %#.18llx\n  cr8    = %#.18llx\n",r->rax,r->rbx,r->rcx,r->rdx,r->rsi,r->rdi,r->rbp,r->r8,r->r9,r->r10,r->r11,r->r12,r->r13,r->r14,r->r15,r->t,_f_msg[r->t],r->ec,r->rip,r->cs,r->rflags,r->rsp,r->ss,cr0,cr2,cr3,cr4,cr8);
	asm_halt_cpu();
}



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD setup_isr(void){
	set_idt_entry(ISR_DIVIDE_BY_ZERO_ERROR,asm_isr0,0x08,0x8e);
	set_idt_entry(ISR_DEBUG,asm_isr1,0x08,0x8e);
	set_idt_entry(ISR_NON_MASKABLE_INTERRUPT,asm_isr2,0x08,0x8e);
	set_idt_entry(ISR_BREAKPOINT,asm_isr3,0x08,0x8e);
	set_idt_entry(ISR_OVERFLOW,asm_isr4,0x08,0x8e);
	set_idt_entry(ISR_BOUND_RANGE_EXCEEDED,asm_isr5,0x08,0x8e);
	set_idt_entry(ISR_INVALID_OPCODE,asm_isr6,0x08,0x8e);
	set_idt_entry(ISR_DEVICE_NOT_AVAILABLE,asm_isr7,0x08,0x8e);
	set_idt_entry(ISR_DOUBLE_FAULT,asm_isr8,0x08,0x8e);
	set_idt_entry(ISR_COPROCESSOR_SEGMENT_OVERRUN,asm_isr9,0x08,0x8e);
	set_idt_entry(ISR_INVALID_TSS,asm_isr10,0x08,0x8e);
	set_idt_entry(ISR_SEGMENT_NOT_PRESENT,asm_isr11,0x08,0x8e);
	set_idt_entry(ISR_STACK_SEGMENT_FAULT,asm_isr12,0x08,0x8e);
	set_idt_entry(ISR_GENERAL_PROTECTION_FAULT,asm_isr13,0x08,0x8e);
	set_idt_entry(ISR_PAGE_FAULT,asm_isr14,0x08,0x8e);
	set_idt_entry(ISR_RESERVED15,asm_isr15,0x08,0x8e);
	set_idt_entry(ISR_X87_FLOATING_POINT_EXCEPTION,asm_isr16,0x08,0x8e);
	set_idt_entry(ISR_ALIGNMENT_CHECK,asm_isr17,0x08,0x8e);
	set_idt_entry(ISR_MACHINE_CHECK,asm_isr18,0x08,0x8e);
	set_idt_entry(ISR_SIMD_FLOATING_POINT_EXCEPTION,asm_isr19,0x08,0x8e);
	set_idt_entry(ISR_VIRTUALIZATION_EXCEPTION,asm_isr20,0x08,0x8e);
	set_idt_entry(ISR_RESERVED21,asm_isr21,0x08,0x8e);
	set_idt_entry(ISR_RESERVED22,asm_isr22,0x08,0x8e);
	set_idt_entry(ISR_RESERVED23,asm_isr23,0x08,0x8e);
	set_idt_entry(ISR_RESERVED24,asm_isr24,0x08,0x8e);
	set_idt_entry(ISR_RESERVED25,asm_isr25,0x08,0x8e);
	set_idt_entry(ISR_RESERVED26,asm_isr26,0x08,0x8e);
	set_idt_entry(ISR_RESERVED27,asm_isr27,0x08,0x8e);
	set_idt_entry(ISR_RESERVED28,asm_isr28,0x08,0x8e);
	set_idt_entry(ISR_RESERVED29,asm_isr29,0x08,0x8e);
	set_idt_entry(ISR_SECURITY_EXCEPTION,asm_isr30,0x08,0x8e);
	set_idt_entry(ISR_RESERVED31,asm_isr31,0x08,0x8e);
}



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h){
	isr_hl[i]=h;
}



void KERNEL_CALL unregiser_isr_handler(uint8_t i){
	isr_hl[i]=NULL;
}
