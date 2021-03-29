#include <shared.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <kmain.h>
#include <memory/paging.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stdint.h>



#define PAGE_GET_ARRAY_INDEX(p) ((p)>>PAGE_4KB_POWER_OF_2)/(sizeof(PageData)*BITS_IN_BYTE)
#define PAGE_GET_BIT_INDEX(p) ((uint8_t)(((p)>>PAGE_4KB_POWER_OF_2)%(sizeof(PageData)*BITS_IN_BYTE)))
#define COMBINE_PAGE_INDEX(i,bi) (((i)*(sizeof(PageData)*BITS_IN_BYTE)+(bi))<<PAGE_4KB_POWER_OF_2)



PageArrayData* _pg_dt;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pm_init(KernelArgs* ka){
	uint64_t sz=((((KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->mmap_l-1].b)+ka->mmap[ka->mmap_l-1].l-1+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE)*sizeof(PageData)+sizeof(PageArrayData)+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2;
	_pg_dt=(PageArrayData*)(void*)ka->n_va;
	while (sz){
		paging_map_page(ka->n_va,ka->n_pa);
		ka->n_va+=PAGE_4KB_SIZE;
		ka->n_pa+=PAGE_4KB_SIZE;
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
	_pg_dt->l=(((KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->mmap_l-1].b)+ka->mmap[ka->mmap_l-1].l-1+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2)+sizeof(PageData)*BITS_IN_BYTE-1)/(sizeof(PageData)*BITS_IN_BYTE);
	for (uint64_t i=0;i<_pg_dt->l;i++){
		_pg_dt->e[i]=0;
	}
	uint64_t i=ka->n_pa;
	uint64_t j=ka->n_pa_idx;
	uint64_t pml4=(uint64_t)(void*)ka->pml4-ka->va_to_pa;
	uint64_t pml4_e=pml4+(ka->t_pg<<PAGE_4KB_POWER_OF_2);
	ASSERT(i<pml4);
	_pg_dt->a=0;
	_pg_dt->ai=PAGE_GET_ARRAY_INDEX(i);
	_pg_dt->abi=PAGE_GET_BIT_INDEX(i);
	while (1){
		_pg_dt->e[PAGE_GET_ARRAY_INDEX(i)]|=1ull<<PAGE_GET_BIT_INDEX(i);
		_pg_dt->a++;
		i+=PAGE_4KB_SIZE;
		while (i>=pml4&&i<pml4_e){
			i+=PAGE_4KB_SIZE;
		}
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



paddr_t KERNEL_CALL pm_get_free(void){
	if (!_pg_dt->a){
		return 0;
	}
	paddr_t o=COMBINE_PAGE_INDEX(_pg_dt->ai,_pg_dt->abi);
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



void KERNEL_CALL pm_set_free(paddr_t pa){
	uint64_t ai=PAGE_GET_ARRAY_INDEX(pa);
	uint8_t abi=PAGE_GET_BIT_INDEX(pa);
	_pg_dt->e[ai]|=1ull<<abi;
	if (ai<_pg_dt->ai){
		_pg_dt->ai=ai;
		if (abi<_pg_dt->abi){
			_pg_dt->abi=abi;
		}
	}
}
