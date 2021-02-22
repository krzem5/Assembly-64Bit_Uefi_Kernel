#ifndef __KERNEL_CPU_TIMER_H__
#define __KERNEL_CPU_TIMER_H__
#include <shared.h>
#include <cpu/acpi.h>



void KERNEL_CALL timer_init(uint64_t b);



uint64_t KERNEL_CALL timer_get_us(void);



uint64_t KERNEL_CALL timer_get_s(void);



#endif
