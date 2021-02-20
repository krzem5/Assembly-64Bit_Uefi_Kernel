#include <libc/stdlib.h>
#include <libc/_libc_internal.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <fatal_error.h>
#include <driver/console.h>



#define MAX_ALLOC_SIZE 0x10000000000
#define PAGE_SIZE 0x1000
#define MIN_BLOCK_SIZE_KEEP 2
#define ALLOC_ALIGNMENT 16
#define ALLOC_PAGES_COUNT 8
#define MEM_BLOCK_GET_PREVIOUS(b) ((MemBlock*)(void*)((b)->a))
#define MEM_BLOCK_GET_FREE(b) ((uint8_t)((b)->b&1))
#define MEM_BLOCK_GET_SIZE(b) ((b)->b&0xfffffffffffffffc)
#define MEM_BLOCK_HAS_NEXT(b) ((uint8_t)((b)->b&2))
#define MEM_BLOCK_GET_NEXT(b) ((MemBlock*)(void*)((uint64_t)(void*)(b)+((b)->b&0xfffffffffffffffc)))
#define MEM_BLOCK_SET_PREVIOUS(p) ((uint64_t)(void*)(p))
#define MEM_BLOCK_SET_FREE 1
#define MEM_BLOCK_SET_SIZE(sz) ((uint64_t)(sz)&0xfffffffffffffffc)
#define MEM_BLOCK_SET_NEXT 2



extern void LIBC_CALL __asm_fill_zero(uint64_t l,uint64_t p);



__set_pg_acquire_func_t __libc_alloc_af=NULL;
__set_pg_release_func_t __libc_alloc_rf=NULL;
typedef struct _MEM_BLOCK{
	uint64_t a;
	uint64_t b;
} MemBlock;
MemBlock __libc_alloc_head={
	MEM_BLOCK_SET_PREVIOUS(NULL),
	MEM_BLOCK_SET_SIZE(0)|MEM_BLOCK_SET_FREE
};



void* LIBC_CALL malloc(size_t sz){
	if (!sz||sz>MAX_ALLOC_SIZE||!__libc_alloc_af){
		return NULL;
	}
	struct _MEM_BLOCK* b=&__libc_alloc_head;
	while (!MEM_BLOCK_GET_FREE(b)||MEM_BLOCK_GET_SIZE(b)<sz){
		if (!MEM_BLOCK_HAS_NEXT(b)){
			size_t pg_c=(sz-(MEM_BLOCK_GET_FREE(b)?MEM_BLOCK_GET_SIZE(b):0)+sizeof(MemBlock)+PAGE_SIZE-1)>>12;
			if (pg_c<ALLOC_PAGES_COUNT){
				pg_c=ALLOC_PAGES_COUNT;
			}
			void* pg=__libc_alloc_af(pg_c);
			b->b|=MEM_BLOCK_SET_NEXT;
			if (MEM_BLOCK_GET_PREVIOUS(b)){
				ASSERT(pg==MEM_BLOCK_GET_NEXT(b));
			}
			if (pg==NULL){
				return NULL;
			}
			if (MEM_BLOCK_GET_FREE(b)){
				b->b=MEM_BLOCK_SET_SIZE((pg_c<<12)+MEM_BLOCK_GET_SIZE(b));
			}
			else{
				b->b|=MEM_BLOCK_SET_NEXT;
				MemBlock* nb=MEM_BLOCK_GET_NEXT(b);
				nb->a=MEM_BLOCK_SET_PREVIOUS(b);
				nb->b=MEM_BLOCK_SET_SIZE(pg_c<<12)|MEM_BLOCK_SET_FREE;
				b=nb;
			}
			break;
		}
		b=MEM_BLOCK_GET_NEXT(b);
	}
	if (sz%ALLOC_ALIGNMENT){
		sz+=ALLOC_ALIGNMENT-(sz%ALLOC_ALIGNMENT);
	}
	uint64_t b_sz=MEM_BLOCK_GET_SIZE(b);
	b->b=MEM_BLOCK_SET_SIZE(sz);
	if (b_sz>sz){
		b->b|=MEM_BLOCK_SET_NEXT;
		MemBlock* nb=MEM_BLOCK_GET_NEXT(b);
		nb->a=MEM_BLOCK_SET_PREVIOUS(b);
		nb->b=MEM_BLOCK_SET_SIZE(b_sz-sz);
	}
	return (void*)((uint64_t)(void*)b+sizeof(MemBlock));
}



void* LIBC_CALL calloc(size_t c,size_t sz){
	uint64_t t=sz*c;
	if (t/sz!=c){
		fatal_error("Multiplication Overflow!\n");
		return NULL;
	}
	if (t%ALLOC_ALIGNMENT){
		t+=ALLOC_ALIGNMENT-(t%ALLOC_ALIGNMENT);
	}
	void* o=malloc(t);
	if (!o){
		fatal_error("Not Enought Memory!\n");
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
}



void LIBC_CALL __set_pg_func(__set_pg_acquire_func_t af,__set_pg_release_func_t rf){
	__libc_alloc_af=af;
	__libc_alloc_rf=rf;
}
