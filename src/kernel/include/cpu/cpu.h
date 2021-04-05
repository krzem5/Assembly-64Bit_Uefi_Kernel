#ifndef __KERNEL_CPU_CPU_H__
#define __KERNEL_CPU_CPU_H__ 1
#include <shared.h>
#include <stdint.h>



typedef struct __CPU{
	struct __CPU* s;
	uint8_t id;
	uint8_t idx;
	void* rsp0;
	uint64_t l_s_tm;
	uint32_t apic;
	uint32_t apic_tpus;
} cpu_t;



extern void KERNEL_CALL asm_cpu_setup_data(cpu_t* dt);



extern cpu_t* KERNEL_CALL asm_current_cpu(void);



void KERNEL_CALL cpu_init(void);



#endif
