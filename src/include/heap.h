#ifndef __KERNEL_HEAP_H__
#define __KERNEL_HEAP_H__ 1
#include <libc/stdint.h>
#include <kmain.h>



void init_heap(KernelArgs* ka);



void* heap_alloc(uint64_t sz,uint8_t a);



#endif
