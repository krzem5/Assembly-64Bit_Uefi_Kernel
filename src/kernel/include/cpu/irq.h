#ifndef __KERNEL_CPU_IRQ_H__
#define __KERNEL_CPU_IRQ_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <stdint.h>



typedef void (KERNEL_CALL *irq_handler_t)(registers_t* r);



extern void KERNEL_CALL asm_irq0(void);



extern void KERNEL_CALL asm_irq1(void);



extern void KERNEL_CALL asm_irq2(void);



extern void KERNEL_CALL asm_irq3(void);



extern void KERNEL_CALL asm_irq4(void);



extern void KERNEL_CALL asm_irq5(void);



extern void KERNEL_CALL asm_irq6(void);



extern void KERNEL_CALL asm_irq7(void);



extern void KERNEL_CALL asm_irq8(void);



extern void KERNEL_CALL asm_irq9(void);



extern void KERNEL_CALL asm_irq10(void);



extern void KERNEL_CALL asm_irq11(void);



extern void KERNEL_CALL asm_irq12(void);



extern void KERNEL_CALL asm_irq13(void);



extern void KERNEL_CALL asm_irq14(void);



extern void KERNEL_CALL asm_irq15(void);



void KERNEL_CALL _handle_irq(registers_t* r);



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD setup_irq(void);



void KERNEL_CALL regiser_irq_handler(uint8_t i,irq_handler_t h);



void KERNEL_CALL unregiser_irq_handler(uint8_t i);



#endif
