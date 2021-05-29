#ifndef __KERNEL_PROCESS_LOCK_H__
#define __KERNEL_PROCESS_LOCK_H__ 1
#include <shared.h>
#include <stdint.h>



typedef uint8_t* lock_t;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD lock_init(void);



lock_t KERNEL_CALL lock_create(void);



void KERNEL_CALL lock_acquire(lock_t l);



void KERNEL_CALL lock_release(lock_t l);



#endif
