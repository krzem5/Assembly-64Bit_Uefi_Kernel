#ifndef __KERNEL_KMAIN_H__
#define __KERNEL_KMAIN_H__ 1
#ifndef KERNEL_ARGS_STRUCT_ONLY
#include <types.h>
#endif



typedef struct _KERNEL_ARGS{
	void* vmem;
	uint64_t vmem_l;
} __attribute__((packed))* KernelArgs;



#ifndef KERNEL_ARGS_STRUCT_ONLY
void __attribute__((ms_abi)) kmain(KernelArgs a);



#endif
#endif
