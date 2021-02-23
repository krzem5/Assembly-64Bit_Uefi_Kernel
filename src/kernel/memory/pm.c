#include <shared.h>
#include <cpu/fatal_error.h>
#include <kmain.h>
#include <memory/paging.h>
#include <memory/pm.h>
#include <memory/vm.h>



#define BITS_IN_BYTE 8
#define PAGE_4KB 4096
#define PAGE_4KB_POWER_OF_2 12
#define PAGE_GET_ARRAY_INDEX(p) ((p)>>PAGE_4KB_POWER_OF_2)/(sizeof(PageData)*BITS_IN_BYTE)
#define PAGE_GET_BIT_INDEX(p) ((uint8_t)(((p)>>PAGE_4KB_POWER_OF_2)%(sizeof(PageData)*BITS_IN_BYTE)))
#define COMBINE_PAGE_INDEX(i,bi) (((i)*(sizeof(PageData)*BITS_IN_BYTE)+(bi))<<PAGE_4KB_POWER_OF_2)



PageArrayData* _pg_dt;



void KERNEL_CALL pm_init(KernelArgs* ka){
	uint64_t sz=((((KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->mmap_l-1].b)+ka->mmap[ka->mmap_l-1].l-1+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE)*sizeof(PageData)+sizeof(PageArrayData)+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2;
	_pg_dt=(PageArrayData*)(void*)vm_current_top();
	while (sz){
		paging_set_page(vm_get_top(),ka->n_pa);
		ka->n_pa+=PAGE_4KB;
		sz--;
		if (ka->n_pa>=KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->n_pa_idx].b)+ka->mmap[ka->n_pa_idx].l){
			ka->n_pa_idx++;
			while (KERNEL_MEM_MAP_GET_ACPI(ka->mmap[ka->n_pa_idx].b)){
				ka->n_pa_idx++;
				ASSERT(ka->n_pa_idx<ka->mmap_l);
			}
			ASSERT(ka->n_pa_idx<ka->mmap_l);
			ka->n_pa=KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->n_pa_idx].b);
		}
	}
	_pg_dt->l=(((KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->mmap_l-1].b)+ka->mmap[ka->mmap_l-1].l-1+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE);
	for (uint64_t i=0;i<_pg_dt->l;i++){
		_pg_dt->e[i]=0;
	}
	uint64_t i=ka->n_pa;
	uint64_t j=ka->n_pa_idx;
	_pg_dt->a=0;
	_pg_dt->ai=PAGE_GET_ARRAY_INDEX(i);
	_pg_dt->abi=PAGE_GET_BIT_INDEX(i);
	while (1){
		_pg_dt->e[PAGE_GET_ARRAY_INDEX(i)]|=1ull<<PAGE_GET_BIT_INDEX(i);
		_pg_dt->a++;
		i+=PAGE_4KB;
		if (i>=KERNEL_MEM_MAP_GET_BASE(ka->mmap[j].b)+ka->mmap[j].l){
			j++;
			while (j<KERNEL_MEM_MAP_GET_ACPI(ka->mmap_l&&ka->mmap[j].b)){
				j++;
			}
			if (j==ka->mmap_l){
				break;
			}
			i=KERNEL_MEM_MAP_GET_BASE(ka->mmap[j].b);
		}
	}
}



uint64_t KERNEL_CALL pm_get_free(void){
	if (!_pg_dt->a){
		return 0;
	}
	uint64_t o=COMBINE_PAGE_INDEX(_pg_dt->ai,_pg_dt->abi);
	_pg_dt->e[_pg_dt->ai]&=~(1ull<<_pg_dt->abi);
	_pg_dt->a--;
	if (_pg_dt->a){
		while (!_pg_dt->e[_pg_dt->ai]){
			_pg_dt->ai++;
		}
		_pg_dt->abi=__builtin_ctzll(_pg_dt->e[_pg_dt->ai]);
	}
	return o;
}
