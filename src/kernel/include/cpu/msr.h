#ifndef __KERNEL_CPU_MSR_H__
#define __KERNEL_CPU_MSR_H__ 1
#include <shared.h>



#define MSR_APIC_BASE 0x1b
#define MSR_EFER 0xc0000080
#define MSR_FS_BASE 0xc0000100
#define MSR_GS_BASE 0xc0000101
#define MSR_KERNEL_GS_BASE 0xc0000102



extern uint64_t KERNEL_CALL asm_rdmsr(uint64_t a);



extern void KERNEL_CALL asm_wrmsr(uint64_t a,uint64_t v);



#endif
