#ifndef __KERNEL_CPU_ISR_H__
#define __KERNEL_CPU_ISR_H__
#include <cpu/idt.h>



typedef void (*isr_handler_t)(exception_registers_t* r);



void setup_isr(void);



void regiser_isr_handler(uint8_t i,isr_handler_t h);



void unregiser_isr_handler(uint8_t i);



#endif
