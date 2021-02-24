#ifndef __KERNEL_PROCESS_SCHEDULER_H__
#define __KERNEL_PROCESS_SCHEDULER_H__ 1
#include <shared.h>
#include <stdint.h>



void KERNEL_CALL scheduler_init(void);



void KERNEL_CALL KERNEL_NO_RETURN scheduler_start(void);



void KERNEL_CALL scheduler_tick(uint64_t tm);



#endif
