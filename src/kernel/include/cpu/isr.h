#ifndef __KERNEL_CPU_ISR_H__
#define __KERNEL_CPU_ISR_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <stdint.h>



#define DIVIDE_BY_ZERO_ERROR 0
#define DEBUG 1
#define NON_MASKABLE_INTERRUPT 2
#define BREAKPOINT 3
#define OVERFLOW 4
#define BOUND_RANGE_EXCEEDED 5
#define INVALID_OPCODE 6
#define DEVICE_NOT_AVAILABLE 7
#define DOUBLE_FAULT 8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define INVALID_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_SEGMENT_FAULT 12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14
#define RESERVED15 15
#define X87_FLOATING_POINT_EXCEPTION 16
#define ALIGNMENT_CHECK 17
#define MACHINE_CHECK 18
#define SIMD_FLOATING_POINT_EXCEPTION 19
#define VIRTUALIZATION_EXCEPTION 20
#define RESERVED21 21
#define RESERVED22 22
#define RESERVED23 23
#define RESERVED24 24
#define RESERVED25 25
#define RESERVED26 26
#define RESERVED27 27
#define RESERVED28 28
#define RESERVED29 29
#define SECURITY_EXCEPTION 30
#define RESERVED31 31



typedef uint8_t (KERNEL_CALL *isr_handler_t)(registers_t* r);



extern void KERNEL_CALL _asm_isr0(void);



extern void KERNEL_CALL _asm_isr1(void);



extern void KERNEL_CALL _asm_isr2(void);



extern void KERNEL_CALL _asm_isr3(void);



extern void KERNEL_CALL _asm_isr4(void);



extern void KERNEL_CALL _asm_isr5(void);



extern void KERNEL_CALL _asm_isr6(void);



extern void KERNEL_CALL _asm_isr7(void);



extern void KERNEL_CALL _asm_isr8(void);



extern void KERNEL_CALL _asm_isr9(void);



extern void KERNEL_CALL _asm_isr10(void);



extern void KERNEL_CALL _asm_isr11(void);



extern void KERNEL_CALL _asm_isr12(void);



extern void KERNEL_CALL _asm_isr13(void);



extern void KERNEL_CALL _asm_isr14(void);



extern void KERNEL_CALL _asm_isr15(void);



extern void KERNEL_CALL _asm_isr16(void);



extern void KERNEL_CALL _asm_isr17(void);



extern void KERNEL_CALL _asm_isr18(void);



extern void KERNEL_CALL _asm_isr19(void);



extern void KERNEL_CALL _asm_isr20(void);



extern void KERNEL_CALL _asm_isr21(void);



extern void KERNEL_CALL _asm_isr22(void);



extern void KERNEL_CALL _asm_isr23(void);



extern void KERNEL_CALL _asm_isr24(void);



extern void KERNEL_CALL _asm_isr25(void);



extern void KERNEL_CALL _asm_isr26(void);



extern void KERNEL_CALL _asm_isr27(void);



extern void KERNEL_CALL _asm_isr28(void);



extern void KERNEL_CALL _asm_isr29(void);



extern void KERNEL_CALL _asm_isr30(void);



extern void KERNEL_CALL _asm_isr31(void);



void KERNEL_CALL _handle_isr(registers_t* r);



void KERNEL_CALL setup_isr(void);



void KERNEL_CALL regiser_isr_handler(uint8_t i,isr_handler_t h);



void KERNEL_CALL unregiser_isr_handler(uint8_t i);



#endif
