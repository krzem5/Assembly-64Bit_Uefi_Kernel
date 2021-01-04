#ifndef __KERNEL_KMAIN_H__
#define __KERNEL_KMAIN_H__ 1
#ifndef KERNEL_ARGS_STRUCT_ONLY
#include <types.h>
#endif



typedef struct __KERNEL_ARGS{
	uint32_t* vmem;
	uint64_t vmem_l;
	uint64_t vmem_w;
	uint64_t vmem_h;
	uint64_t mmap_l;
	struct __KERNEL_ARGS_MEM_ENTRY{
		uint64_t b;
		uint64_t l;
	} mmap[];
} KernelArgs;
typedef struct __KERNEL_ARGS_MEM_ENTRY KernelArgsMemEntry;



#ifndef KERNEL_ARGS_STRUCT_ONLY
void __attribute__((ms_abi)) kmain(KernelArgs ka);



#endif
#endif
