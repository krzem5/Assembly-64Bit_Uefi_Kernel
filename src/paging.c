#include <paging.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <kmain.h>
#include <libc/_libc_internal.h>
#include <fatal_error.h>
#include <driver/console.h>



#define PAGE_4KB 0x1000
#define PAGE_4KB_POWER_OF_2 12
#define PAGE_2MB 0x200000
#define PAGE_2MB_POWER_OF_2 21
#define GET_FREE(pg) ((pg)->n>>63);
#define GET_NEXT(pg) ((pg)->n&0x7fffffffffffffff);


struct _PAGE_DATA{
	struct _PAGE_DATA* p;
	struct _PAGE_DATA* n;
};
struct _PAGE_ARRAY_DATA{
	struct _PAGE_DATA* e;
	uint32_t l;
};
uint64_t* _pg_pml4;
uint64_t _pg_t_pg;
uint64_t _pg_k_pg;
uint64_t _pg_pg_id;
uint64_t _pg_mmap_l;
KernelArgsMemEntry* _pg_mmap;
uint64_t _pg_t_mem;
uint64_t _pg_mm_t;



void* _temp_pg_get_kernel_page(uint64_t c){
	if (c>=PAGE_2MB){
		return NULL;
	}
	void* o=(void*)_pg_mm_t;
	_pg_mm_t+=c<<12;
	if (_pg_mm_t>=_pg_t_mem||(uint64_t)o>_pg_mm_t){
		(void)CHECK_NOT_NULL(!"Memory Overflow");
	}
	return o;
}



void paging_temp_init(KernelArgs* ka){
	_pg_pml4=ka->pml4;
	_pg_t_pg=ka->t_pg;
	_pg_k_pg=ka->k_pg;
	_pg_pg_id=ka->u_pg;
	_pg_mmap_l=ka->mmap_l;
	_pg_mmap=ka->mmap;
	_pg_t_mem=0;
	for (uint64_t i=0;i<_pg_mmap_l;i++){
		_pg_t_mem+=(_pg_mmap+i)->l;
	}
	_pg_mm_t=(uint64_t)_pg_pml4+(_pg_t_pg<<12);
	__set_pg_func(_temp_pg_get_kernel_page,NULL);
}



void paging_init(KernelArgs* ka){
	uint64_t dt_pg=(((_pg_t_mem+PAGE_4KB)>>PAGE_4KB_POWER_OF_2)*sizeof(struct _PAGE_DATA)+sizeof(struct _PAGE_ARRAY_DATA)+PAGE_2MB-1)>>PAGE_2MB_POWER_OF_2;
	console_log("Total Mem 4Kb Pages: %llu (%llu * 2Mb)\n",(_pg_t_mem+4095)>>12,dt_pg);
}
