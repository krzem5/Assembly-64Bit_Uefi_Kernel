#ifndef __KERNEL_PROCESS_SCHEDULER_H__
#define __KERNEL_PROCESS_SCHEDULER_H__ 1
#include <shared.h>
#include <stdint.h>



extern uint8_t scheduler_ready;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD scheduler_init(void);



void KERNEL_CALL scheduler_start(void);



void KERNEL_CALL scheduler_yield(void);



#endif
