#ifndef __KERNEL_CPU_HPET_TIMER_H__
#define __KERNEL_CPU_HPET_TIMER_H__
#include <shared.h>
#include <cpu/acpi.h>



#define HPET_TIMER_TARGET_FREQUENCY 1000ull



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD hpet_timer_init(uint64_t b);



uint64_t KERNEL_CALL hpet_timer_get_us(void);



void KERNEL_CALL hpet_timer_spinwait(uint64_t us);



#endif
