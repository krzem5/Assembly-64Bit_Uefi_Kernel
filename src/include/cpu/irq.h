#ifndef __KERNEL_CPU_IRQ_H__
#define __KERNEL_CPU_IRQ_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <libc/stdint.h>



typedef void (KERNEL_CALL *irq_handler_t)(registers_t* r);



void KERNEL_CALL _handle_irq(registers_t* r);



void KERNEL_CALL setup_irq(void);



void KERNEL_CALL regiser_irq_handler(uint8_t i,irq_handler_t h);



void KERNEL_CALL unregiser_irq_handler(uint8_t i);



#endif
