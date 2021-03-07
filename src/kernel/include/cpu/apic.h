#ifndef __KERNEL_CPU_APIC_H__
#define __KERNEL_CPU_APIC_H__ 1
#include <shared.h>
#include <stdint.h>



void KERNEL_CALL apic_init(uint64_t a);



uint64_t KERNEL_CALL apic_base(void);



void KERNEL_CALL apic_enable(void);



uint8_t KERNEL_CALL apic_get_id(void);



void KERNEL_CALL apic_setup_timer(void);



#endif
