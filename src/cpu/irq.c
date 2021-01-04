#include <types.h>
#include <cpu/irq.h>
#include <cpu/ports.h>
#include <cpu/idt.h>



extern void _asm_irq0(void);
extern void _asm_irq1(void);
extern void _asm_irq2(void);
extern void _asm_irq3(void);
extern void _asm_irq4(void);
extern void _asm_irq5(void);
extern void _asm_irq6(void);
extern void _asm_irq7(void);
extern void _asm_irq8(void);
extern void _asm_irq9(void);
extern void _asm_irq10(void);
extern void _asm_irq11(void);
extern void _asm_irq12(void);
extern void _asm_irq13(void);
extern void _asm_irq14(void);
extern void _asm_irq15(void);



irq_handler_t irq_hl[16]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};



void _handle_irq(registers_t* r){
	if (irq_hl[r->t-0x20]!=NULL){
		irq_hl[r->t-0x20](r);
	}
	if (r->t>=0x28){
		port_out(0xa0,0x20);
	}
	port_out(0x20,0x20);
}



void setup_irq(void){
	port_out(0x20,0x11);
	port_out(0xa0,0x11);
	port_out(0x21,0x20);
	port_out(0xa1,0x28);
	port_out(0x21,0x04);
	port_out(0xa1,0x02);
	port_out(0x21,0x01);
	port_out(0xa1,0x01);
	port_out(0x21,0x0);
	port_out(0xa1,0x0);
	set_idt_entry(32,_asm_irq0,0x08,0x8e);
	set_idt_entry(33,_asm_irq1,0x08,0x8e);
	set_idt_entry(34,_asm_irq2,0x08,0x8e);
	set_idt_entry(35,_asm_irq3,0x08,0x8e);
	set_idt_entry(36,_asm_irq4,0x08,0x8e);
	set_idt_entry(37,_asm_irq5,0x08,0x8e);
	set_idt_entry(38,_asm_irq6,0x08,0x8e);
	set_idt_entry(39,_asm_irq7,0x08,0x8e);
	set_idt_entry(40,_asm_irq8,0x08,0x8e);
	set_idt_entry(41,_asm_irq9,0x08,0x8e);
	set_idt_entry(42,_asm_irq10,0x08,0x8e);
	set_idt_entry(43,_asm_irq11,0x08,0x8e);
	set_idt_entry(44,_asm_irq12,0x08,0x8e);
	set_idt_entry(45,_asm_irq13,0x08,0x8e);
	set_idt_entry(46,_asm_irq14,0x08,0x8e);
	set_idt_entry(47,_asm_irq15,0x08,0x8e);
}



void regiser_irq_handler(uint8_t i,irq_handler_t h){
	irq_hl[i]=h;
}



void unregiser_irq_handler(uint8_t i){
	irq_hl[i]=NULL;
}
