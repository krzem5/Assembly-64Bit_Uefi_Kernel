#ifndef __KERNEL_PROCESS_THREAD_H__
#define __KERNEL_PROCESS_THREAD_H__ 1
#include <shared.h>
#include <stdint.h>



#define THREAD_PRIORITY_NORMAL 0



typedef uint32_t tid_t;
typedef void (KERNEL_CALL *thread_start_t)(void* arg);



tid_t KERNEL_CALL create_thread(thread_start_t a,uint8_t p,void* arg);



#endif
