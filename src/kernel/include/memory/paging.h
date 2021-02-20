#ifndef __KERNEL_MEMORY_PAGING_H__
#define __KERNEL_MEMORY_PAGING_H__ 1
#include <shared.h>
#include <kmain.h>
#include <stdint.h>



void KERNEL_CALL paging_init(KernelArgs* ka);



void* KERNEL_CALL paging_alloc_pages(uint64_t c);



#endif
