#include <shared.h>
#include <cpu/fatal_error.h>
#include <cpu/isr.h>
#include <gfx/console.h>
#include <kmain.h>
#include <memory/paging.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stddef.h>
#include <stdint.h>



#define MAX_PROCESS_RAM 0x40000000
#define PAGE_GET_ARRAY_INDEX(a) ((a)>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)
#define PAGE_GET_BIT_INDEX(a) ((uint8_t)(((a)>>PAGE_4KB_POWER_OF_2)%(sizeof(VmMemMapData)*BITS_IN_BYTE)))
#define COMBINE_PAGE_INDEX(i,bi) (((i)*(sizeof(VmMemMapData)*BITS_IN_BYTE)+(bi))<<PAGE_4KB_POWER_OF_2)



vaddr_t _n_va;
VmMemMap* _vm_dt;



uint8_t KERNEL_CALL _vm_pg_f_handler(registers_t* r){
	vaddr_t cr2=(vaddr_t)asm_get_cr2()&0xfffffffffffff000;
	if (cr2>=_vm_dt->b&&PAGE_GET_ARRAY_INDEX(cr2-_vm_dt->b)<(MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)&&_vm_dt->e[PAGE_GET_ARRAY_INDEX(cr2-_vm_dt->b)]&(1ull<<PAGE_GET_BIT_INDEX(cr2-_vm_dt->b))){
		paddr_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought Memory!\n");
			return 0;
		}
		paging_map_page(cr2,pa);
		return 1;
	}
	return 0;
}



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD vm_init(KernelArgs* ka){
	_n_va=ka->n_va;
	uint64_t pg_c=(((MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)*sizeof(VmMemMapData)+sizeof(sizeof(VmMemMap)))+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2;
	_vm_dt=(VmMemMap*)(void*)_vm_dt->n_va;
	while (pg_c){
		uint64_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought memory!");
		}
		paging_map_page(_n_va,pa);
		_n_va+=PAGE_4KB_SIZE;
		pg_c--;
	}
	_vm_dt->n_va=_n_va;
	_vm_dt->b=__KERNEL_BASE_VA__;
	for (uint64_t i=0;i<(MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE);i++){
		_vm_dt->e[i]=0;
	}
	regiser_isr_handler(ISR_PAGE_FAULT,_vm_pg_f_handler);
}



vaddr_t KERNEL_CALL vm_reserve(uint64_t c){
	vaddr_t o=_vm_dt->n_va;
	while (c){
		_vm_dt->e[PAGE_GET_ARRAY_INDEX(_vm_dt->n_va-_vm_dt->b)]|=1ull<<(PAGE_GET_BIT_INDEX(_vm_dt->n_va-_vm_dt->b));
		_vm_dt->n_va+=PAGE_4KB_SIZE;
		c--;
	}
	return o;
}



vaddr_t KERNEL_CALL vm_commit(uint64_t c){
	vaddr_t o=_vm_dt->n_va;
	while (c){
		paddr_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought Memory!\n");
			return 0;
		}
		_vm_dt->e[PAGE_GET_ARRAY_INDEX(_vm_dt->n_va-_vm_dt->b)]|=1ull<<(PAGE_GET_BIT_INDEX(_vm_dt->n_va-_vm_dt->b));
		paging_map_page(_vm_dt->n_va,pa);
		_vm_dt->n_va+=PAGE_4KB_SIZE;
		c--;
	}
	return o;
}



void KERNEL_CALL vm_identity_map(vaddr_t a,uint64_t c){
	while (c){
		paging_map_page(a,a);
		a+=PAGE_4KB_SIZE;
		c--;
	}
}



void KERNEL_CALL vm_release(vaddr_t va,uint64_t c){
	while (c){
		paddr_t pa=paging_reverse_translate(va);
		if (pa){
			pm_set_free(pa);
			paging_unmap_page(va);
		}
		_vm_dt->e[PAGE_GET_ARRAY_INDEX(va-_vm_dt->b)]&=~(1ull<<(PAGE_GET_BIT_INDEX(va-_vm_dt->b)));
		va+=PAGE_4KB_SIZE;
		c--;
	}
}
