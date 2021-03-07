#ifndef __KERNEL_CPU_CPU_H__
#define __KERNEL_CPU_CPU_H__ 1
#include <shared.h>
#include <stdint.h>



typedef struct __CPU{
	struct __CPU* s;
	uint8_t id;
	uint8_t idx;
	void* rsp0;
	uint32_t apic;
} cpu_t;



void KERNEL_CALL cpu_init(void);



void KERNEL_CALL cpu_ap_init(cpu_t* cpu);



extern cpu_t* KERNEL_CALL current_cpu(void);



#endif
