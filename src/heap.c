#include <heap.h>
#include <types.h>
#include <driver/console.h>



// extern uint64_t __KERNEL_OFFSET__[];
// extern uint64_t __KERNEL_END__[];
// extern uint64_t __MAX_KERNEL_HEAP_SIZE__[];
#define __KERNEL_OFFSET__ 0
#define __KERNEL_END__ 0
#define __MAX_KERNEL_HEAP_SIZE__ 0x1000000
uint64_t _kheap_ptr;



void init_heap(void){
	_kheap_ptr=(uint64_t)__KERNEL_OFFSET__+(uint64_t)__KERNEL_END__;
}



void* heap_alloc(uint64_t sz,uint8_t a){
	_kheap_ptr+=(_kheap_ptr%a!=0?a-(_kheap_ptr%a):0);
	void* o=(void*)_kheap_ptr;
	_kheap_ptr+=sz;
	if (_kheap_ptr-(uint64_t)__KERNEL_OFFSET__-(uint64_t)__KERNEL_END__>=(uint64_t)__MAX_KERNEL_HEAP_SIZE__){
		console_error("Kernel Heap Overflow!");
		for(;;);
	}
	return o;
}



void* kmalloc(size_t sz);



void* kcalloc(size_t c,size_t sz);



void* krealloc(void* p,size_t sz);



void kfree(void* p);
