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
	if (r->t>=208){
		console_error("APIC:\n  rax    = %#.18llx\n  rbx    = %#.18llx\n  rcx    = %#.18llx\n  rdx    = %#.18llx\n  rsi    = %#.18llx\n  rdi    = %#.18llx\n  rbp    = %#.18llx\n  r8     = %#.18llx\n  r9     = %#.18llx\n  r10    = %#.18llx\n  r11    = %#.18llx\n  r12    = %#.18llx\n  r13    = %#.18llx\n  r14    = %#.18llx\n  r15    = %#.18llx\n  t      = %#.4llx\n  ec     = %#.4llx\n  rip    = %#.18llx\n  cs     = %#.6llx\n  rflags = %#.8llx\n  rsp    = %#.18llx\n  ss     = %#.6llx\n",r->rax,r->rbx,r->rcx,r->rdx,r->rsi,r->rdi,r->rbp,r->r8,r->r9,r->r10,r->r11,r->r12,r->r13,r->r14,r->r15,r->t,r->ec,r->rip,r->cs,r->rflags,r->rsp,r->ss);
		asm_halt_cpu();
	}
	if (isr_hl[r->t]&&isr_hl[r->t](r)){
		return;
	}
	console_error("Error:\n  rax    = %#.18llx\n  rbx    = %#.18llx\n  rcx    = %#.18llx\n  rdx    = %#.18llx\n  rsi    = %#.18llx\n  rdi    = %#.18llx\n  rbp    = %#.18llx\n  r8     = %#.18llx\n  r9     = %#.18llx\n  r10    = %#.18llx\n  r11    = %#.18llx\n  r12    = %#.18llx\n  r13    = %#.18llx\n  r14    = %#.18llx\n  r15    = %#.18llx\n  t      = %#.4llx (%s)\n  ec     = %#.4llx\n  rip    = %#.18llx\n  cs     = %#.6llx\n  rflags = %#.8llx\n  rsp    = %#.18llx\n  ss     = %#.6llx\n",r->rax,r->rbx,r->rcx,r->rdx,r->rsi,r->rdi,r->rbp,r->r8,r->r9,r->r10,r->r11,r->r12,r->r13,r->r14,r->r15,r->t,_f_msg[r->t],r->ec,r->rip,r->cs,r->rflags,r->rsp,r->ss);
	asm_halt_cpu();
}



void KERNEL_CALL setup_isr(void){
	set_idt_entry(0,asm_isr0,0x08,0x8e);
	set_idt_entry(1,asm_isr1,0x08,0x8e);
	set_idt_entry(2,asm_isr2,0x08,0x8e);
	set_idt_entry(3,asm_isr3,0x08,0x8e);
	set_idt_entry(4,asm_isr4,0x08,0x8e);
	set_idt_entry(5,asm_isr5,0x08,0x8e);
	set_idt_entry(6,asm_isr6,0x08,0x8e);
	set_idt_entry(7,asm_isr7,0x08,0x8e);
	set_idt_entry(8,asm_isr8,0x08,0x8e);
	set_idt_entry(9,asm_isr9,0x08,0x8e);
	set_idt_entry(10,asm_isr10,0x08,0x8e);
	set_idt_entry(11,asm_isr11,0x08,0x8e);
	set_idt_entry(12,asm_isr12,0x08,0x8e);
	set_idt_entry(13,asm_isr13,0x08,0x8e);
	set_idt_entry(14,asm_isr14,0x08,0x8e);
	set_idt_entry(15,asm_isr15,0x08,0x8e);
	set_idt_entry(16,asm_isr16,0x08,0x8e);
	set_idt_entry(17,asm_isr17,0x08,0x8e);
	set_idt_entry(18,asm_isr18,0x08,0x8e);
	set_idt_entry(19,asm_isr19,0x08,0x8e);
	set_idt_entry(20,asm_isr20,0x08,0x8e);
	set_idt_entry(21,asm_isr21,0x08,0x8e);
	set_idt_entry(22,asm_isr22,0x08,0x8e);
	set_idt_entry(23,asm_isr23,0x08,0x8e);
	set_idt_entry(24,asm_isr24,0x08,0x8e);
	set_idt_entry(25,asm_isr25,0x08,0x8e);
	set_idt_entry(26,asm_isr26,0x08,0x8e);
	set_idt_entry(27,asm_isr27,0x08,0x8e);
	set_idt_entry(28,asm_isr28,0x08,0x8e);
	set_idt_entry(29,asm_isr29,0x08,0x8e);
	set_idt_entry(30,asm_isr30,0x08,0x8e);
	set_idt_entry(31,asm_isr31,0x08,0x8e);
	set_idt_entry(208,asm_isr208,0x08,0x8e);
	set_idt_entry(209,asm_isr209,0x08,0x8e);
	set_idt_entry(210,asm_isr210,0x08,0x8e);
	set_idt_entry(211,asm_isr211,0x08,0x8e);
	set_idt_entry(212,asm_isr212,0x08,0x8e);
	set_idt_entry(213,asm_isr213,0x08,0x8e);
}



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h){
	isr_hl[i]=h;
}



void KERNEL_CALL unregiser_isr_handler(uint8_t i){
	isr_hl[i]=NULL;
}
