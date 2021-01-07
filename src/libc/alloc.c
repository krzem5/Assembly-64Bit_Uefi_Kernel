#include <libc/stdlib.h>
#include <libc/_libc_internal.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>



#define MAX_ALLOC_SIZE 0x10000000000
#define PAGE_SIZE 0x1000
#define MIN_BLOCK_SIZE_KEEP 2



__set_pg_acquire_func_t __libc_alloc_af=NULL;
__set_pg_release_func_t __libc_alloc_rf=NULL;
struct _MEM_BLOCK{
	struct _MEM_BLOCK* p;
	struct _MEM_BLOCK* n;
	size_t t_sz;
	size_t sz;
	bool f;
} __libc_alloc_head={
	NULL,
	NULL,
	0,
	0,
	true
};



void* malloc(size_t sz){
	if (sz==0||sz>MAX_ALLOC_SIZE||__libc_alloc_af==NULL){
		return NULL;
	}
	struct _MEM_BLOCK* n=&__libc_alloc_head;
	while (n->f==false||n->sz<sz){
		if (n->n==NULL){
			size_t pg_c=(sz+sizeof(struct _MEM_BLOCK)+PAGE_SIZE-1)>>12;
			void* pg=__libc_alloc_af(pg_c);
			if (pg==NULL){
				return NULL;
			}
			n->n=pg;
			n->n->p=n;
			n=n->n;
			n->t_sz=pg_c<<12;
			break;
		}
		n=n->n;
	}
	n->sz=sz;
	n->f=false;
	if (n->t_sz-sz-2*sizeof(struct _MEM_BLOCK)>=MIN_BLOCK_SIZE_KEEP){
		n->n=n+sizeof(struct _MEM_BLOCK)+sz;
		n->n->p=n;
		n->n->n=NULL;
		n->n->t_sz=n->t_sz-sz-sizeof(struct _MEM_BLOCK);
		n->n->sz=n->n->t_sz-sizeof(struct _MEM_BLOCK);
		n->n->f=true;
		n->t_sz=sz+sizeof(struct _MEM_BLOCK);
	}
	else{
		n->n=NULL;
	}
	return (void*)n->n+sizeof(struct _MEM_BLOCK);
}



void* calloc(size_t c,size_t sz){
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



void* realloc(void* p,size_t sz){
	return NULL;
}



void free(void* p){
	if (p==NULL){
		return;
	}
}



void __set_pg_func(__set_pg_acquire_func_t af,__set_pg_release_func_t rf){
	__libc_alloc_af=af;
	__libc_alloc_rf=rf;
}
