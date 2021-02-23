#include <_libc_internal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __KERNEL__
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <memory/vm.h>
#endif



#define MAX_ALLOC_SIZE 0x10000000000
#define PAGE_SIZE 0x1000
#define MIN_BLOCK_SIZE_KEEP 2
#define ALLOC_ALIGNMENT 16
#define ALLOC_PAGES_COUNT 8
#define MEM_BLOCK_GET_PREVIOUS(bl) ((MemBlock*)(void*)((bl)->a))
#define MEM_BLOCK_GET_FREE(bl) ((uint8_t)((bl)->b&1))
#define MEM_BLOCK_HAS_NEXT(bl) ((uint8_t)((bl)->b&2))
#define MEM_BLOCK_GET_NEXT(bl) ((MemBlock*)(void*)((uint64_t)(void*)(bl)+((bl)->b&0xfffffffffffffffc)+sizeof(MemBlock)))
#define MEM_BLOCK_GET_SIZE(bl) ((bl)->b&0xfffffffffffffffc)
#define MEM_BLOCK_SET_PREVIOUS(p) ((uint64_t)(void*)(p))
#define MEM_BLOCK_SET_FREE 1
#define MEM_BLOCK_SET_NEXT 2
#define MEM_BLOCK_COPY_NEXT(bl) ((bl)->b&2)
#define MEM_BLOCK_SET_SIZE(sz) ((uint64_t)(sz)&0xfffffffffffffffc)



extern void LIBC_CALL __asm_fill_zero(uint64_t l,uint64_t p);



typedef struct _MEM_BLOCK{
	uint64_t a;
	uint64_t b;
} MemBlock;
MemBlock* __libc_alloc_head=NULL;



void* LIBC_CALL malloc(size_t sz){
	if (sz%ALLOC_ALIGNMENT){
		sz+=ALLOC_ALIGNMENT-(sz%ALLOC_ALIGNMENT);
	}
	if (!sz||sz>MAX_ALLOC_SIZE){
		return NULL;
	}
	if (!__libc_alloc_head){
#ifdef __KERNEL__
		void* pg=(void*)vm_reserve(ALLOC_PAGES_COUNT);
#else
		void* pg=NULL;
#endif
		if (pg==NULL){
			return NULL;
		}
		__libc_alloc_head=(MemBlock*)pg;
		__libc_alloc_head->a=MEM_BLOCK_SET_PREVIOUS(NULL);
		__libc_alloc_head->b=MEM_BLOCK_SET_FREE|MEM_BLOCK_SET_SIZE((ALLOC_PAGES_COUNT<<12)-sizeof(MemBlock));
	}
	struct _MEM_BLOCK* b=__libc_alloc_head;
	while (!MEM_BLOCK_GET_FREE(b)||MEM_BLOCK_GET_SIZE(b)<sz){
		if (!MEM_BLOCK_HAS_NEXT(b)){
			size_t pg_c=(sz-(MEM_BLOCK_GET_FREE(b)?MEM_BLOCK_GET_SIZE(b):0)+sizeof(MemBlock)+PAGE_SIZE-1)>>12;
			if (pg_c<ALLOC_PAGES_COUNT){
				pg_c=ALLOC_PAGES_COUNT;
			}
#ifdef __KERNEL__
			void* pg=(void*)vm_reserve(pg_c);
#else
			void* pg=NULL;
#endif
			if (pg==NULL){
				return NULL;
			}
#ifdef __KERNEL__
			if (MEM_BLOCK_GET_PREVIOUS(b)){
				ASSERT(pg==MEM_BLOCK_GET_NEXT(b));
			}
#endif
			if (MEM_BLOCK_GET_FREE(b)){
				b->b=MEM_BLOCK_SET_SIZE((pg_c<<12)+MEM_BLOCK_GET_SIZE(b));
			}
			else{
				b->b|=MEM_BLOCK_SET_NEXT;
				MemBlock* nb=MEM_BLOCK_GET_NEXT(b);
				nb->a=MEM_BLOCK_SET_PREVIOUS(b);
				nb->b=MEM_BLOCK_SET_FREE|MEM_BLOCK_SET_SIZE((pg_c<<12)-sizeof(MemBlock));
				b=nb;
			}
			break;
		}
		b=MEM_BLOCK_GET_NEXT(b);
	}
	uint64_t b_sz=MEM_BLOCK_GET_SIZE(b);
	b->b=MEM_BLOCK_SET_SIZE(sz);
	if (b_sz>sz){
		b->b|=MEM_BLOCK_SET_NEXT;
		MemBlock* nb=MEM_BLOCK_GET_NEXT(b);
		nb->a=MEM_BLOCK_SET_PREVIOUS(b);
		nb->b=MEM_BLOCK_SET_FREE|MEM_BLOCK_SET_SIZE(b_sz-sz-sizeof(MemBlock));
	}
	return (void*)((uint64_t)(void*)b+sizeof(MemBlock));
}



void* LIBC_CALL calloc(size_t c,size_t sz){
	uint64_t t=sz*c;
	if (t/sz!=c){
#ifdef __KERNEL__
		fatal_error("Multiplication Overflow!\n");
#endif
		return NULL;
	}
	if (t%ALLOC_ALIGNMENT){
		t+=ALLOC_ALIGNMENT-(t%ALLOC_ALIGNMENT);
	}
	void* o=malloc(t);
	if (!o){
#ifdef __KERNEL__
		fatal_error("Not enough Memory!\n");
#endif
		return o;
	}
	__asm_fill_zero(t,(uint64_t)o);
	return o;
}



void* LIBC_CALL realloc(void* p,size_t sz){
	return NULL;
}



void LIBC_CALL free(void* p){
	if (!p){
		return;
	}
	MemBlock* b=(MemBlock*)(void*)((uint64_t)p-sizeof(MemBlock));
	b->b|=MEM_BLOCK_SET_FREE;
	if (MEM_BLOCK_HAS_NEXT(b)){
		MemBlock* nb=MEM_BLOCK_GET_NEXT(b);
		if (MEM_BLOCK_GET_FREE(nb)){
			b->b=MEM_BLOCK_SET_FREE|MEM_BLOCK_COPY_NEXT(nb)|MEM_BLOCK_SET_SIZE(MEM_BLOCK_GET_SIZE(b)+MEM_BLOCK_GET_SIZE(nb)+sizeof(MemBlock));
		}
	}
	if (MEM_BLOCK_GET_PREVIOUS(b)&&MEM_BLOCK_GET_FREE(MEM_BLOCK_GET_PREVIOUS(b))){
		MemBlock* pb=MEM_BLOCK_GET_PREVIOUS(b);
		pb->b=MEM_BLOCK_SET_FREE|MEM_BLOCK_COPY_NEXT(b)|MEM_BLOCK_SET_SIZE(MEM_BLOCK_GET_SIZE(pb)+MEM_BLOCK_GET_SIZE(b)+sizeof(MemBlock));
	}
}
