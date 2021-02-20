#include <shared.h>
#include <cpu/isr.h>
#include <stdint.h>
#include <stddef.h>
#include <cpu/idt.h>
#include <driver/console.h>
#include <fatal_error.h>
#include <shared.h>



extern void KERNEL_CALL _asm_isr0(void);
extern void KERNEL_CALL _asm_isr1(void);
extern void KERNEL_CALL _asm_isr2(void);
extern void KERNEL_CALL _asm_isr3(void);
extern void KERNEL_CALL _asm_isr4(void);
extern void KERNEL_CALL _asm_isr5(void);
extern void KERNEL_CALL _asm_isr6(void);
extern void KERNEL_CALL _asm_isr7(void);
extern void KERNEL_CALL _asm_isr8(void);
extern void KERNEL_CALL _asm_isr9(void);
extern void KERNEL_CALL _asm_isr10(void);
extern void KERNEL_CALL _asm_isr11(void);
extern void KERNEL_CALL _asm_isr12(void);
extern void KERNEL_CALL _asm_isr13(void);
extern void KERNEL_CALL _asm_isr14(void);
extern void KERNEL_CALL _asm_isr15(void);
extern void KERNEL_CALL _asm_isr16(void);
extern void KERNEL_CALL _asm_isr17(void);
extern void KERNEL_CALL _asm_isr18(void);
extern void KERNEL_CALL _asm_isr19(void);
extern void KERNEL_CALL _asm_isr20(void);
extern void KERNEL_CALL _asm_isr21(void);
extern void KERNEL_CALL _asm_isr22(void);
extern void KERNEL_CALL _asm_isr23(void);
extern void KERNEL_CALL _asm_isr24(void);
extern void KERNEL_CALL _asm_isr25(void);
extern void KERNEL_CALL _asm_isr26(void);
extern void KERNEL_CALL _asm_isr27(void);
extern void KERNEL_CALL _asm_isr28(void);
extern void KERNEL_CALL _asm_isr29(void);
extern void KERNEL_CALL _asm_isr30(void);
extern void KERNEL_CALL _asm_isr31(void);



isr_handler_t isr_hl[32];
char* _f_msg[32]={
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



void KERNEL_CALL _handle_isr(registers_t* r){
	if (isr_hl[r->t]){
		isr_hl[r->t](r);
	}
	else{
		console_error("FatalError\n");
		console_error("Error:\n  rax    = %#.18llx\n  rbx    = %#.18llx\n  rcx    = %#.18llx\n  rdx    = %#.18llx\n  rsi    = %#.18llx\n  rdi    = %#.18llx\n  rbp    = %#.18llx\n  r8     = %#.18llx\n  r9     = %#.18llx\n  r10    = %#.18llx\n  r11    = %#.18llx\n  r12    = %#.18llx\n  r13    = %#.18llx\n  r14    = %#.18llx\n  r15    = %#.18llx\n  t      = %#.4llx (%s)\n  ec     = %#.4llx\n  rip    = %#.18llx\n  cs     = %#.6llx\n  rflags = %#.8llx\n  rsp    = %#.18llx\n  ss     = %#.6llx\n",r->rax,r->rbx,r->rcx,r->rdx,r->rsi,r->rdi,r->rbp,r->r8,r->r9,r->r10,r->r11,r->r12,r->r13,r->r14,r->r15,r->t,_f_msg[r->t],r->ec,r->rip,r->cs,r->rflags,r->rsp,r->ss);
		asm_halt_cpu();
	}
}



void KERNEL_CALL setup_isr(void){
	set_idt_entry(0,_asm_isr0,0x08,0x8e);
	set_idt_entry(1,_asm_isr1,0x08,0x8e);
	set_idt_entry(2,_asm_isr2,0x08,0x8e);
	set_idt_entry(3,_asm_isr3,0x08,0x8e);
	set_idt_entry(4,_asm_isr4,0x08,0x8e);
	set_idt_entry(5,_asm_isr5,0x08,0x8e);
	set_idt_entry(6,_asm_isr6,0x08,0x8e);
	set_idt_entry(7,_asm_isr7,0x08,0x8e);
	set_idt_entry(8,_asm_isr8,0x08,0x8e);
	set_idt_entry(9,_asm_isr9,0x08,0x8e);
	set_idt_entry(10,_asm_isr10,0x08,0x8e);
	set_idt_entry(11,_asm_isr11,0x08,0x8e);
	set_idt_entry(12,_asm_isr12,0x08,0x8e);
	set_idt_entry(13,_asm_isr13,0x08,0x8e);
	set_idt_entry(14,_asm_isr14,0x08,0x8e);
	set_idt_entry(15,_asm_isr15,0x08,0x8e);
	set_idt_entry(16,_asm_isr16,0x08,0x8e);
	set_idt_entry(17,_asm_isr17,0x08,0x8e);
	set_idt_entry(18,_asm_isr18,0x08,0x8e);
	set_idt_entry(19,_asm_isr19,0x08,0x8e);
	set_idt_entry(20,_asm_isr20,0x08,0x8e);
	set_idt_entry(21,_asm_isr21,0x08,0x8e);
	set_idt_entry(22,_asm_isr22,0x08,0x8e);
	set_idt_entry(23,_asm_isr23,0x08,0x8e);
	set_idt_entry(24,_asm_isr24,0x08,0x8e);
	set_idt_entry(25,_asm_isr25,0x08,0x8e);
	set_idt_entry(26,_asm_isr26,0x08,0x8e);
	set_idt_entry(27,_asm_isr27,0x08,0x8e);
	set_idt_entry(28,_asm_isr28,0x08,0x8e);
	set_idt_entry(29,_asm_isr29,0x08,0x8e);
	set_idt_entry(30,_asm_isr30,0x08,0x8e);
	set_idt_entry(31,_asm_isr31,0x08,0x8e);
	for (uint8_t i=0;i<32;i++){
		isr_hl[i]=NULL;
	}
}



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h){
	isr_hl[i]=h;
}



void KERNEL_CALL unregiser_isr_handler(uint8_t i){
	isr_hl[i]=NULL;
}
