#ifndef __KERNEL_CPU_HPET_TIMER_H__
#define __KERNEL_CPU_HPET_TIMER_H__
#include <shared.h>
#include <cpu/acpi.h>



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD hpet_timer_init(uint64_t b);



void KERNEL_CALL hpet_timer_set_frequency(uint64_t f);



uint64_t KERNEL_CALL hpet_timer_get_us(void);



uint64_t KERNEL_CALL hpet_timer_get_s(void);



void KERNEL_CALL hpet_timer_spinwait(uint64_t us);



#endif
