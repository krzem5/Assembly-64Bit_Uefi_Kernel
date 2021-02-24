#ifndef __KERNEL_PROCESS_PROCESS_H__
#define __KERNEL_PROCESS_PROCESS_H__ 1
#include <shared.h>
#include <stdint.h>



typedef uint32_t pid_t;
typedef struct __PROCESS_DATA{
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rsp;
	uint64_t rbp;
	uint64_t rip;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rflags;
	uint64_t cs;
	uint64_t ss;
} process_data_t;
typedef struct __PROCESS{
	uint8_t f;
	pid_t id;
	process_data_t dt;
} process_t;



void KERNEL_CALL process_init(void);



process_t* KERNEL_CALL process_create(uint8_t p);



#endif
