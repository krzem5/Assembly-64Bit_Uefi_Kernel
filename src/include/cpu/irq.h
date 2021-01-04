#ifndef __KERNEL_CPU_IRQ_H__
#define __KERNEL_CPU_IRQ_H__
#include <cpu/idt.h>



typedef void (*irq_handler_t)(registers_t* r);



void setup_irq(void);



void regiser_irq_handler(uint8_t i,irq_handler_t h);



void unregiser_irq_handler(uint8_t i);



#endif
