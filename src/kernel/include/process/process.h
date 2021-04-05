#ifndef __KERNEL_PROCESS_PROCESS_H__
#define __KERNEL_PROCESS_PROCESS_H__ 1
#include <shared.h>
#include <stdint.h>



#define MAX_PROCESS_THREADS 256



typedef uint32_t pid_t;
typedef struct __PROCESS{
	uint8_t f;
	pid_t id;
	uint32_t tll;
	void* tl[MAX_PROCESS_THREADS];
} process_t;



extern process_t* kernel_process;



void KERNEL_CALL process_init(void);



process_t* KERNEL_CALL process_create(uint8_t p);



#endif
