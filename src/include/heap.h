#ifndef __KERNEL_HEAP_H__
#define __KERNEL_HEAP_H__
#include <types.h>



void init_heap(void);



void* heap_alloc(uint64_t sz,uint8_t a);



void* kmalloc(size_t sz);



void* kcalloc(size_t c,size_t sz);



void* krealloc(void* p,size_t sz);



void kfree(void* p);



#endif
