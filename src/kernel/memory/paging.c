#include <shared.h>
#include <_libc_internal.h>
#include <cpu/fatal_error.h>
#include <kmain.h>
#include <memory/paging.h>
#include <memory/pm.h>
#include <stddef.h>
#include <stdint.h>



#define GET_PML4_INDEX(va) (((va)>>39)&0x1ff)
#define GET_PDP_INDEX(va) (((va)>>30)&0x1ff)
#define GET_PD_INDEX(va) (((va)>>21)&0x1ff)
#define GET_PT_INDEX(va) (((va)>>12)&0x1ff)
#define PAGE_TABLE_VIRTUAL_TO_PHYSICAL(va) ((va)-_pg_va_to_pa)
#define PAGE_TABLE_GET_VIRTUAL_ADDRESS(pg) (((pg)&0xfffffffffffffe00)+_pg_va_to_pa)
#define PAGE_TABLE_SIZE 4096



uint64_t* _pg_pml4;
uint64_t _pg_t_pg;
uint64_t _pg_u_pg;
uint64_t _pg_va_to_pa;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD paging_init(KernelArgs* ka){
	_pg_pml4=ka->pml4;
	_pg_t_pg=ka->t_pg;
	_pg_u_pg=ka->u_pg;
	_pg_va_to_pa=ka->va_to_pa;
}



void KERNEL_CALL paging_map_page(vaddr_t va,paddr_t pa){
	ASSERT(!(pa&(PAGE_4KB_SIZE-1)));
	ASSERT(!(va&(PAGE_4KB_SIZE-1)));
	if (!(_pg_pml4[GET_PML4_INDEX(va)]&PAGE_DIR_PRESENT)){
		_pg_pml4[GET_PML4_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		_pg_u_pg++;
	}
	uint64_t* t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(_pg_pml4[GET_PML4_INDEX(va)]);
	if (!(t[GET_PDP_INDEX(va)]&PAGE_DIR_PRESENT)){
		t[GET_PDP_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		_pg_u_pg++;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PDP_INDEX(va)]);
	if (!(t[GET_PD_INDEX(va)]&PAGE_DIR_PRESENT)){
		t[GET_PD_INDEX(va)]=PAGE_TABLE_VIRTUAL_TO_PHYSICAL((uint64_t)(void*)_pg_pml4+_pg_u_pg*PAGE_TABLE_SIZE)|PAGE_DIR_READ_WRITE|PAGE_DIR_PRESENT;
		_pg_u_pg++;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PD_INDEX(va)]);
	ASSERT(!(t[GET_PT_INDEX(va)]&PAGE_PRESENT));
	t[GET_PT_INDEX(va)]=pa|PAGE_READ_WRITE|PAGE_PRESENT;
	__asm__ volatile("invlpg (%0)"::"r"(va):"memory");
}



paddr_t KERNEL_CALL paging_get_phys_pml4(void){
	return PAGE_TABLE_VIRTUAL_TO_PHYSICAL((vaddr_t)(void*)_pg_pml4);
}



paddr_t KERNEL_CALL paging_reverse_translate(vaddr_t va){
	ASSERT(!(va&(PAGE_4KB_SIZE-1)));
	if (!(_pg_pml4[GET_PML4_INDEX(va)]&PAGE_DIR_PRESENT)){
		return 0;
	}
	uint64_t* t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(_pg_pml4[GET_PML4_INDEX(va)]);
	if (!(t[GET_PDP_INDEX(va)]&PAGE_DIR_PRESENT)){
		return 0;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PDP_INDEX(va)]);
	if (!(t[GET_PD_INDEX(va)]&PAGE_DIR_PRESENT)){
		return 0;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PD_INDEX(va)]);
	if (!(t[GET_PT_INDEX(va)]&PAGE_PRESENT)){
		return 0;
	}
	return t[GET_PT_INDEX(va)]&0xfffffffffffff000;
}



void KERNEL_CALL paging_unmap_page(vaddr_t va){
	ASSERT(!(va&(PAGE_4KB_SIZE-1)));
	if (!(_pg_pml4[GET_PML4_INDEX(va)]&PAGE_DIR_PRESENT)){
		return;
	}
	uint64_t* t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(_pg_pml4[GET_PML4_INDEX(va)]);
	if (!(t[GET_PDP_INDEX(va)]&PAGE_DIR_PRESENT)){
		return;
	}
	t=(uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PDP_INDEX(va)]);
	if (!(t[GET_PD_INDEX(va)]&PAGE_DIR_PRESENT)){
		return;
	}
	((uint64_t*)(void*)PAGE_TABLE_GET_VIRTUAL_ADDRESS(t[GET_PD_INDEX(va)]))[GET_PT_INDEX(va)]=0;
}



void KERNEL_CALL paging_identity_map(vaddr_t a,uint64_t c){
	while (c){
		paging_map_page(a,a);
		a+=PAGE_4KB_SIZE;
		c--;
	}
}



void KERNEL_CALL paging_identity_unmap(vaddr_t a,uint64_t c){
	while (c){
		paging_unmap_page(a);
		a+=PAGE_4KB_SIZE;
		c--;
	}
}
