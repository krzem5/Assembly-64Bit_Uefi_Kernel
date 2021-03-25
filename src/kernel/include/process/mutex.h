#ifndef __KERNEL_PROCESS_MUTEX_H__
#define __KERNEL_PROCESS_MUTEX_H__ 1
#include <shared.h>
#include <stdint.h>



typedef uint8_t* mutex_t;



extern uint64_t KERNEL_CALL asm_mutex_acquire(mutex_t s);



extern void KERNEL_CALL asm_mutex_release(mutex_t s);



void KERNEL_CALL mutex_init(void);



mutex_t KERNEL_CALL mutex_create(void);



#endif
