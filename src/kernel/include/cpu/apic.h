#ifndef __KERNEL_CPU_APIC_H__
#define __KERNEL_CPU_APIC_H__ 1
#include <shared.h>
#include <cpu/idt.h>
#include <stdint.h>



#define APIC_SPURIOUS_INTERRUPT 208
#define APIC_TIMER_INTERRUPT 209
#define APIC_LINT0_INTERRUPT 210
#define APIC_LINT1_INTERRUPT 211
#define APIC_ERROR_INTERRUPT 212
#define IPI_DELIVERY_FIXED 0
#define IPI_DELIVERY_LOWEST_PRIORITY 1
#define IPI_DELIVERY_SMI 2
#define IPI_DELIVERY_NMI 4
#define IPI_DELIVERY_INIT 5
#define IPI_DELIVERY_STARTUP 6
#define IPI_DESTINATION_PHYSICAL 0
#define IPI_DESTINATION_LOGICAL 8
#define IPI_LEVEL_DEASSERT 0
#define IPI_LEVEL_ASSERT 64
#define IPI_TRIGGER_EDGE 0
#define IPI_TRIGGER_LEVEL 128
#define IPI_DESTINATION_SHORTHAND_NONE 0
#define IPI_DESTINATION_SHORTHAND_SELF 1024
#define IPI_DESTINATION_SHORTHAND_ALL_WITH_SELF 2048
#define IPI_DESTINATION_SHORTHAND_ALL_WITHOUT_SELF 3072



typedef struct __APIC_INTERRUPT_REGISTERS{
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rflags;
	uint64_t t;
} __attribute__((packed)) apic_interrupt_registers_t;



extern void KERNEL_CALL asm_apic_setup(uint64_t apic);



extern void KERNEL_CALL asm_apic_spurious_int(void);



extern void KERNEL_CALL asm_apic_timer_int(void);



extern void KERNEL_CALL asm_apic_lint0_int(void);



extern void KERNEL_CALL asm_apic_lint1_int(void);



extern void KERNEL_CALL asm_apic_err_int(void);



void KERNEL_CALL _handle_apic_int(apic_interrupt_registers_t* r);



void KERNEL_CALL apic_init(void);



void KERNEL_CALL apic_send_ipi(uint64_t apic,uint16_t t,uint8_t v,uint8_t d);



#endif
