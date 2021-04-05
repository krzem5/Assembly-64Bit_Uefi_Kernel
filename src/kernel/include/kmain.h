#ifndef __KERNEL_KMAIN_H__
#define __KERNEL_KMAIN_H__ 1
#include <shared.h>
#ifndef KERNEL_ARGS_STRUCT_ONLY
#include <stdint.h>
#endif



#define KERNEL_MEM_MAP_GET_BASE(b) ((b)&0xfffffffffffffffe)
#define KERNEL_MEM_MAP_GET_ACPI(b) ((b)&1)



typedef struct __KERNEL_ARGS_MEM_ENTRY{
	uint64_t b;
	uint64_t l;
} KernelArgsMemEntry;
typedef struct __KERNEL_ARGS{
	uint32_t* vmem;
	uint64_t vmem_l;
	uint64_t vmem_w;
	uint64_t vmem_h;
	void* apic;
	void* fadt;
	void* hpet;
	void* idt;
	uint64_t* pml4;
	uint64_t t_pg;
	uint64_t u_pg;
	uint64_t n_va;
	uint64_t n_pa;
	uint64_t n_pa_idx;
	uint64_t va_to_pa;
	uint64_t k_sp;
	uint64_t mmap_l;
	KernelArgsMemEntry mmap[];
} KernelArgs;



#ifdef KERNEL_ARGS_STRUCT_ONLY
typedef void (KERNEL_CALL *kmain)(KernelArgs* ka);
#else
#define __KERNEL_BASE_VA__ ((uint64_t)___KERNEL_BASE_VA__)



extern uint64_t ___KERNEL_BASE_VA__[];



void KERNEL_CALL kmain(KernelArgs* ka);



#endif
#endif
