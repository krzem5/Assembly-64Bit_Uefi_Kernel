#ifndef __KERNEL_CPU_ISR_H__
#define __KERNEL_CPU_ISR_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <stdint.h>



#define ISR_DIVIDE_BY_ZERO_ERROR 0
#define ISR_DEBUG 1
#define ISR_NON_MASKABLE_INTERRUPT 2
#define ISR_BREAKPOINT 3
#define ISR_OVERFLOW 4
#define ISR_BOUND_RANGE_EXCEEDED 5
#define ISR_INVALID_OPCODE 6
#define ISR_DEVICE_NOT_AVAILABLE 7
#define ISR_DOUBLE_FAULT 8
#define ISR_COPROCESSOR_SEGMENT_OVERRUN 9
#define ISR_INVALID_TSS 10
#define ISR_SEGMENT_NOT_PRESENT 11
#define ISR_STACK_SEGMENT_FAULT 12
#define ISR_GENERAL_PROTECTION_FAULT 13
#define ISR_PAGE_FAULT 14
#define ISR_RESERVED15 15
#define ISR_X87_FLOATING_POINT_EXCEPTION 16
#define ISR_ALIGNMENT_CHECK 17
#define ISR_MACHINE_CHECK 18
#define ISR_SIMD_FLOATING_POINT_EXCEPTION 19
#define ISR_VIRTUALIZATION_EXCEPTION 20
#define ISR_RESERVED21 21
#define ISR_RESERVED22 22
#define ISR_RESERVED23 23
#define ISR_RESERVED24 24
#define ISR_RESERVED25 25
#define ISR_RESERVED26 26
#define ISR_RESERVED27 27
#define ISR_RESERVED28 28
#define ISR_RESERVED29 29
#define ISR_SECURITY_EXCEPTION 30
#define ISR_RESERVED31 31



typedef uint8_t (KERNEL_CALL *isr_handler_t)(registers_t* r);



extern void KERNEL_CALL asm_isr0(void);



extern void KERNEL_CALL asm_isr1(void);



extern void KERNEL_CALL asm_isr2(void);



extern void KERNEL_CALL asm_isr3(void);



extern void KERNEL_CALL asm_isr4(void);



extern void KERNEL_CALL asm_isr5(void);



extern void KERNEL_CALL asm_isr6(void);



extern void KERNEL_CALL asm_isr7(void);



extern void KERNEL_CALL asm_isr8(void);



extern void KERNEL_CALL asm_isr9(void);



extern void KERNEL_CALL asm_isr10(void);



extern void KERNEL_CALL asm_isr11(void);



extern void KERNEL_CALL asm_isr12(void);



extern void KERNEL_CALL asm_isr13(void);



extern void KERNEL_CALL asm_isr14(void);



extern void KERNEL_CALL asm_isr15(void);



extern void KERNEL_CALL asm_isr16(void);



extern void KERNEL_CALL asm_isr17(void);



extern void KERNEL_CALL asm_isr18(void);



extern void KERNEL_CALL asm_isr19(void);



extern void KERNEL_CALL asm_isr20(void);



extern void KERNEL_CALL asm_isr21(void);



extern void KERNEL_CALL asm_isr22(void);



extern void KERNEL_CALL asm_isr23(void);



extern void KERNEL_CALL asm_isr24(void);



extern void KERNEL_CALL asm_isr25(void);



extern void KERNEL_CALL asm_isr26(void);



extern void KERNEL_CALL asm_isr27(void);



extern void KERNEL_CALL asm_isr28(void);



extern void KERNEL_CALL asm_isr29(void);



extern void KERNEL_CALL asm_isr30(void);



extern void KERNEL_CALL asm_isr31(void);



void KERNEL_CALL _handle_isr(registers_t* r);



void KERNEL_CALL setup_isr(void);



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h);



void KERNEL_CALL unregiser_isr_handler(uint8_t i);



#endif
