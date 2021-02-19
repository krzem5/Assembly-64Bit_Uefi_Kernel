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
#define ALLOC_PAGES_COUNT 16
#define MEM_BLOCK_GET_PREVIOUS(b) ((MemBlock*)(void*)((b)->a&0x7fffffffffffffff))
#define MEM_BLOCK_GET_FREE(b) ((uint8_t)((b)->a>>63))
#define MEM_BLOCK_GET_SIZE(b) ((b)->b&0x7fffffffffffffff)
#define MEM_BLOCK_HAS_NEXT(b) ((uint8_t)((b)->b>>63))
#define MEM_BLOCK_GET_NEXT(b) ((MemBlock*)(void*)((uint64_t)(void*)(b)+((b)->b&0x7fffffffffffffff)))
#define MEM_BLOCK_SET_PREVIOUS(p) ((uint64_t)(void*)(p))
#define MEM_BLOCK_SET_FREE 0x8000000000000000
#define MEM_BLOCK_SET_SIZE(sz) ((uint64_t)(sz))
#define MEM_BLOCK_SET_NEXT 0x8000000000000000



__set_pg_acquire_func_t __libc_alloc_af=NULL;
__set_pg_release_func_t __libc_alloc_rf=NULL;
typedef struct _MEM_BLOCK{
	uint64_t a;
	uint64_t b;
} MemBlock;
MemBlock __libc_alloc_head={
	MEM_BLOCK_SET_PREVIOUS(NULL)|MEM_BLOCK_SET_FREE,
	MEM_BLOCK_SET_SIZE(0)
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
			// n->n=pg;
			// n->n->p=n;
			// n=n->n;
			// n->sz=pg_c<<12;
			break;
		}
		b=MEM_BLOCK_GET_NEXT(b);
	}
	b->b=MEM_BLOCK_SET_SIZE(sz);
	// if (n->sz-sz-2*sizeof(struct _MEM_BLOCK)>=MIN_BLOCK_SIZE_KEEP){
	// 	n->n=n+sizeof(struct _MEM_BLOCK)+sz;
	// 	n->n->p=n;
	// 	n->n->n=NULL;
	// 	n->n->t_sz=n->t_sz-sz-sizeof(struct _MEM_BLOCK);
	// 	n->n->sz=n->n->t_sz-sizeof(struct _MEM_BLOCK);
	// 	n->n->f=true;
	// 	n->t_sz=sz+sizeof(struct _MEM_BLOCK);
	// }
	// else{
	// 	n->n=NULL;
	// }
	void* o=(void*)((uint64_t)(void*)b+sizeof(MemBlock));
	console_log("%p\n",(void*)((uint64_t)(void*)b+sizeof(MemBlock)));
	return o;
}



void* LIBC_CALL calloc(size_t c,size_t sz){
	c*=sz;
	void* o=malloc(c);
	if (o==NULL){
		return o;
	}
	while (c){
		*((uint8_t*)o+c)=0;
		c--;
	}
	return o;
}



void* LIBC_CALL realloc(void* p,size_t sz){
	return NULL;
}



void LIBC_CALL free(void* p){
	// if (p==NULL){
	// 	return;
	// }
}



void LIBC_CALL __set_pg_func(__set_pg_acquire_func_t af,__set_pg_release_func_t rf){
	__libc_alloc_af=af;
	__libc_alloc_rf=rf;
}
