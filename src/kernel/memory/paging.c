#include <shared.h>
#include <_libc_internal.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <kmain.h>
#include <memory/paging.h>
#include <memory/pm.h>
#include <stddef.h>
#include <stdint.h>



#define BITS_IN_BYTE 8
#define PAGE_4KB 0x1000
#define PAGE_4KB_POWER_OF_2 12
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



uint64_t* _pg_pml4;
uint64_t _pg_t_pg;
uint64_t _pg_u_pg;
uint64_t _pg_va_to_pa;



void KERNEL_CALL paging_init(KernelArgs* ka){
	_pg_pml4=ka->pml4;
	_pg_t_pg=ka->t_pg;
	_pg_u_pg=ka->u_pg;
	_pg_va_to_pa=ka->va_to_pa;
}



void KERNEL_CALL paging_set_page(vaddr_t va,paddr_t pa){
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
	__asm__ volatile("invlpg (%0)"::"r"(va):"memory");
}
