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



#define BITS_IN_BYTE 8
#define PAGE_4KB 4096
#define PAGE_4KB_POWER_OF_2 12
#define MIN_RAM_RESERVE_FOR_SYSTEM 0x40000000
#define PAGE_GET_ARRAY_INDEX(a) ((a)>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)
#define PAGE_GET_BIT_INDEX(a) ((uint8_t)(((a)>>PAGE_4KB_POWER_OF_2)%(sizeof(VmMemMapData)*BITS_IN_BYTE)))
#define COMBINE_PAGE_INDEX(i,bi) (((i)*(sizeof(VmMemMapData)*BITS_IN_BYTE)+(bi))<<PAGE_4KB_POWER_OF_2)



uint64_t MAX_PROCESS_RAM=0x10000000000;
uint64_t _n_va=-1;
VmMemMap* _vm_dt;



uint8_t KERNEL_CALL _vm_pg_f_handler(registers_t* r){
	uint64_t cr2=asm_get_cr2()&0xfffffffffffff000;
	if (cr2>=_vm_dt->b&&PAGE_GET_ARRAY_INDEX(cr2-_vm_dt->b)<(MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)&&_vm_dt->e[PAGE_GET_ARRAY_INDEX(cr2-_vm_dt->b)]&(1ull<<PAGE_GET_BIT_INDEX(cr2-_vm_dt->b))){
		uint64_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought Memory!\n");
			return 0;
		}
		console_log("Comming Page: %llx -> %llx\n",pa,cr2);
		paging_set_page(cr2,pa);
		return 1;
	}
	return 0;
}



void KERNEL_CALL vm_init(KernelArgs* ka){
	_n_va=ka->n_va;
	uint64_t mx=KERNEL_MEM_MAP_GET_BASE(ka->mmap[ka->mmap_l-1].b)+ka->mmap[ka->mmap_l-1].l-MIN_RAM_RESERVE_FOR_SYSTEM;
	if (mx<MAX_PROCESS_RAM){
		MAX_PROCESS_RAM=mx;
	}
}



void KERNEL_CALL vm_after_pm_init(KernelArgs* ka){
	uint64_t pg_c=(((MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE)*sizeof(VmMemMapData)+sizeof(sizeof(VmMemMap)))+PAGE_4KB-1)>>PAGE_4KB_POWER_OF_2;
	_vm_dt=(VmMemMap*)(void*)vm_current_top();
	while (pg_c){
		uint64_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought memory!");
		}
		paging_set_page(vm_get_top(),pa);
		pg_c--;
	}
	_vm_dt->n_va=_n_va;
	_vm_dt->b=__KERNEL_BASE_VA__;
	for (uint64_t i=0;i<(MAX_PROCESS_RAM>>PAGE_4KB_POWER_OF_2)/(sizeof(VmMemMapData)*BITS_IN_BYTE);i++){
		_vm_dt->e[i]=0;
	}
	regiser_isr_handler(PAGE_FAULT,_vm_pg_f_handler);
}



void* KERNEL_CALL vm_reserve(uint64_t c){
	void* o=(void*)vm_current_top();
	while (c){
		console_log("Reserve Page: %llx\n",vm_current_top());
		uint64_t va=vm_get_top()-_vm_dt->b;
		_vm_dt->e[PAGE_GET_ARRAY_INDEX(va)]|=1ull<<(PAGE_GET_BIT_INDEX(va));
		c--;
	}
	return o;
}



void* KERNEL_CALL vm_commit(uint64_t c){
	void* o=(void*)vm_current_top();
	while (c){
		uint64_t pa=pm_get_free();
		if (!pa){
			fatal_error("Not enought Memory!\n");
			return NULL;
		}
		console_log("Reserving & Commiting Page: %llx -> %llx\n",pa,vm_current_top());
		uint64_t va=vm_current_top()-_vm_dt->b;
		_vm_dt->e[PAGE_GET_ARRAY_INDEX(va)]|=1ull<<(PAGE_GET_BIT_INDEX(va));
		paging_set_page(vm_get_top(),pa);
		c--;
	}
	return o;
}



uint64_t KERNEL_CALL vm_current_top(void){
	return _n_va;
}



uint64_t KERNEL_CALL vm_get_top(void){
	uint64_t o=_n_va;
	_n_va+=PAGE_4KB;
	return o;
}
