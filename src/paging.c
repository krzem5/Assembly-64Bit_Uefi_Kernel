#include <paging.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <kmain.h>
#include <libc/_libc_internal.h>
#include <fatal_error.h>
#include <driver/console.h>



#define BITS_IN_BYTE 8
#define PAGE_4KB 0x1000
#define PAGE_4KB_POWER_OF_2 12
#define PAGE_2MB 0x200000
#define PAGE_2MB_POWER_OF_2 21
#define GET_PML4_INDEX(va) (((va)>>39)&0x1ff)
#define GET_PDP_INDEX(va) (((va)>>30)&0x1ff)
#define GET_PD_INDEX(va) (((va)>>21)&0x1ff)
#define GET_PT_INDEX(va) (((va)>>12)&0x1ff)
#define PAGE_PRESENT 1
#define PAGE_READ_WRITE 2
#define PAGE_USER 4
#define PAGE_WRITE_THROUGH 8
#define PAGE_CACHE_DISABLED 16
#define PAGE_ACCESSED 32
#define PAGE_DIRTY 64
#define PAGE_GLOBAL 256
#define PAGE_DIR_PRESENT 1
#define PAGE_DIR_READ_WRITE 2
#define PAGE_DIR_USER 4
#define PAGE_DIR_WRITE_THROUGH 8
#define PAGE_DIR_CACHE_DISABLED 16
#define PAGE_DIR_ACCESSED 32
#define PAGE_DIR_SIZE 128
#define PAGE_TABLE_VIRTUAL_TO_PHYSICAL(va) ((va)-_pg_va_to_pa)
#define PAGE_TABLE_GET_VIRTUAL_ADDRESS(pg) (((pg)&0xfffffffffffffe00)+_pg_va_to_pa)
#define PAGE_TABLE_SIZE 4096



typedef uint64_t PageData;
typedef struct _PAGE_ARRAY_DATA PageArrayData;



struct _PAGE_ARRAY_DATA{
	uint64_t l;
	uint64_t a;
	uint64_t ai;
	uint64_t abi;
	PageData e[];
};



uint64_t* _pg_pml4;
uint64_t _pg_t_pg;
uint64_t _pg_u_pg;
uint64_t _pg_k_pg;
uint64_t _pg_t_mem;
uint64_t _pg_va_to_pa;
PageArrayData* _pg_dt;



void* _temp_pg_get_kernel_page(uint64_t c){
	ASSERT(0);
	return NULL;
}



void paging_temp_init(KernelArgs* ka){
	_pg_pml4=ka->pml4;
	_pg_t_pg=ka->t_pg;
	_pg_u_pg=ka->u_pg;
	_pg_k_pg=ka->k_pg;
	_pg_va_to_pa=ka->va_to_pa;
	_pg_t_mem=0;
	for (uint64_t i=0;i<ka->mmap_l;i++){
		_pg_t_mem+=(ka->mmap+i)->l;
	}
	__set_pg_func(_temp_pg_get_kernel_page,NULL);
}



void set_page(uint64_t va,uint64_t pa){
	ASSERT(!(pa&0xfff));
	if (!(_pg_pml4[GET_PML4_INDEX(va)]&PAGE_DIR_PRESENT)){
		_pg_pml4[GET_PML4_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		console_log("PML4[%llu] = %llx (PA)\n",GET_PML4_INDEX(va),_pg_pml4[GET_PML4_INDEX(va)]&~(PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT));
		_pg_u_pg++;
	}
	uint64_t* t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(_pg_pml4[GET_PML4_INDEX(va)]);
	if (!(t[GET_PDP_INDEX(va)]&PAGE_DIR_PRESENT)){
		t[GET_PDP_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		console_log("PDP[%llu] = %llx (PA)\n",GET_PDP_INDEX(va),t[GET_PDP_INDEX(va)]&~(PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT));
		_pg_u_pg++;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PDP_INDEX(va)]);
	if (!(t[GET_PD_INDEX(va)]&PAGE_DIR_PRESENT)){
		t[GET_PD_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		console_log("PD[%llu] = %llx (PA)\n",GET_PD_INDEX(va),t[GET_PD_INDEX(va)]&~(PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT));
		_pg_u_pg++;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PD_INDEX(va)]);
	ASSERT(!(t[GET_PT_INDEX(va)]&PAGE_PRESENT));
	t[GET_PT_INDEX(va)]=pa|PAGE_READ_WRITE|PAGE_PRESENT;
}



void paging_init(KernelArgs* ka){
	uint64_t mx_mem=ka->mmap[ka->mmap_l-1].b+ka->mmap[ka->mmap_l-1].l-1;
	uint64_t sz=((((mx_mem+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE)*sizeof(PageData)+sizeof(PageArrayData)+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2;
	console_log("Total Mem 4Kb Pages: %llu (%llu * 4Kb)\n",(mx_mem+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2,sz);
	_pg_dt=(PageArrayData*)(void*)ka->n_va;
	while (sz){
		set_page(ka->n_va,ka->n_pa);
		console_log("%llx -> %llx\n",ka->n_pa,ka->n_va);
		ka->n_pa+=PAGE_4KB;
		ka->n_va+=PAGE_4KB;
		sz--;
		if (ka->n_pa>=(ka->mmap[ka->n_pa_idx].b&0x7fffffffffffffff)+ka->mmap[ka->n_pa_idx].l){
			ka->n_pa_idx++;
			ASSERT(ka->n_pa_idx<ka->mmap_l);
			ka->n_pa=ka->mmap[ka->n_pa_idx].b&0x7fffffffffffffff;
		}
	}
	_pg_dt->l=(((_pg_t_mem+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE);
	for (uint64_t i=0;i<_pg_dt->l;i++){
		_pg_dt->e[i]=0;
	}
	uint64_t i=ka->n_pa;
	uint64_t j=ka->n_pa_idx;
	_pg_dt->a=0;
	_pg_dt->ai=(i>>12)/(sizeof(PageData)*BITS_IN_BYTE);
	_pg_dt->abi=(i>>12)%(sizeof(PageData)*BITS_IN_BYTE);
	while (1){
		_pg_dt->e[(i>>12)/(sizeof(PageData)*BITS_IN_BYTE)]|=1<<((i>>12)%(sizeof(PageData)*BITS_IN_BYTE));
		_pg_dt->a++;
		i+=PAGE_4KB;
		if (i>=(ka->mmap[j].b&0x7fffffffffffffff)+ka->mmap[j].l){
			j++;
			if (j==ka->mmap_l){
				break;
			}
			i=ka->mmap[j].b&0x7fffffffffffffff;
		}
	}
}
