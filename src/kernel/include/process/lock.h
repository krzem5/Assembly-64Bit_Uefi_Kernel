#ifndef __KERNEL_PROCESS_LOCK_H__
#define __KERNEL_PROCESS_LOCK_H__ 1
#include <shared.h>
#include <stdint.h>



typedef uint8_t* lock_t;



extern uint64_t KERNEL_CALL asm_lock_acquire(lock_t s);



extern void KERNEL_CALL asm_lock_release(lock_t s);



void KERNEL_CALL lock_init(void);



lock_t KERNEL_CALL lock_create(void);



#endif
