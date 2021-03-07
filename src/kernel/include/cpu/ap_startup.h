#ifndef __KERNEL_CPU_AP_STARTUP_H__
#define __KERNEL_CPU_AP_STARTUP_H__ 1
#include <shared.h>
#include <stdint.h>



extern void KERNEL_CALL asm_init_ap_startup_code(uint64_t pml4);



extern void KERNEL_CALL asm_setup_ap_startup_vars(uint64_t cpu_dt);



#endif
