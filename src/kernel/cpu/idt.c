#include <shared.h>
#include <cpu/fatal_error.h>
#include <cpu/idt.h>
#include <kmain.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>



idt_entry_t* idt_l=NULL;
idt_table_t idt={
	4095,
	0
};



void KERNEL_CALL setup_idt(KernelArgs* ka){
	idt_l=(idt_entry_t*)ka->idt;
	idt.b=(uint64_t)idt_l;
	asm_setup_idt();
}



void KERNEL_CALL set_idt_entry(uint8_t i,void* a,uint16_t s,uint8_t f){
	idt_l[i].off16=((uint64_t)a)&0xffff;
	idt_l[i].s=s;
	idt_l[i].f=f;
	idt_l[i].off32=(((uint64_t)a)>>16)&0xffff;
	idt_l[i].off64=((uint64_t)a)>>32;
}
