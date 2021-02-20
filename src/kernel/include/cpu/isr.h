#ifndef __KERNEL_CPU_ISR_H__
#define __KERNEL_CPU_ISR_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <stdint.h>



typedef void (KERNEL_CALL *isr_handler_t)(registers_t* r);



void KERNEL_CALL _handle_isr(registers_t* r);



void KERNEL_CALL setup_isr(void);



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h);



void KERNEL_CALL unregiser_isr_handler(uint8_t i);



#endif
