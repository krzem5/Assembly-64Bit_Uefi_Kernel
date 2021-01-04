#include <types.h>
#include <cpu/isr.h>
#include <cpu/idt.h>
#include <driver/console.h>
#include <fatal_error.h>



extern void _asm_isr0(void);
extern void _asm_isr1(void);
extern void _asm_isr2(void);
extern void _asm_isr3(void);
extern void _asm_isr4(void);
extern void _asm_isr5(void);
extern void _asm_isr6(void);
extern void _asm_isr7(void);
extern void _asm_isr8(void);
extern void _asm_isr9(void);
extern void _asm_isr10(void);
extern void _asm_isr11(void);
extern void _asm_isr12(void);
extern void _asm_isr13(void);
extern void _asm_isr14(void);
extern void _asm_isr15(void);
extern void _asm_isr16(void);
extern void _asm_isr17(void);
extern void _asm_isr18(void);
extern void _asm_isr19(void);
extern void _asm_isr20(void);
extern void _asm_isr21(void);
extern void _asm_isr22(void);
extern void _asm_isr23(void);
extern void _asm_isr24(void);
extern void _asm_isr25(void);
extern void _asm_isr26(void);
extern void _asm_isr27(void);
extern void _asm_isr28(void);
extern void _asm_isr29(void);
extern void _asm_isr30(void);
extern void _asm_isr31(void);



isr_handler_t isr_hl[32]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
char* _f_msg[32]={
	"Division By Zero",
	"Debug",
	"Non-Maskable-Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Bound-Range",
	"Invalid-Opcode",
	"Device-Not-Available",
	"Double-Fault",
	"Reserved",
	"Invalid-TSS",
	"Segment-Not-Present",
	"Stack",
	"General-Protection",
	"Page-Fault",
	"Reserved",
	"x87 Floating-Point Exception-Pending",
	"Alignment-Check",
	"Machine-Check",
	"SIMD Floating-Point",
	"Reserved",
	"Control Protection",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	"Reserved"
};



void _handle_isr(exception_registers_t* r){
	if (isr_hl[r->t]!=NULL){
		isr_hl[r->t](r);
	}
	else{
		console_error(_f_msg[r->t]);
		console_continue(" Exception Occured (");
		// print_uint32(r->t);
		console_continue("). Stopping Execution.\r\nRIP = ");
		// print_laddr(r->rip);
		console_continue(", Error Code = ");
		// print_uint32(r->ec);
		asm_halt_cpu();
	}
}



void setup_isr(void){
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
}



void regiser_isr_handler(uint8_t i,isr_handler_t h);



void unregiser_isr_handler(uint8_t i);
