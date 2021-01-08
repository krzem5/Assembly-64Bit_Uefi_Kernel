#ifndef __KERNEL_CPU_ISR_H__
#define __KERNEL_CPU_ISR_H__ 1
#include <cpu/idt.h>
#include <libc/stdint.h>



typedef void (*isr_handler_t)(registers_t* r);



void __attribute__((ms_abi)) _handle_isr(registers_t* r);



void setup_isr(void);



void regiser_isr_handler(uint8_t i,isr_handler_t h);



void unregiser_isr_handler(uint8_t i);



#endif
