#ifndef __KERNEL_MEMORY_PAGING_H__
#define __KERNEL_MEMORY_PAGING_H__ 1
#include <shared.h>
#ifndef __EFI_LOADER__
#include <kmain.h>
#include <memory/pm.h>
#include <memory/vm.h>
#endif
#include <stdint.h>



#define BITS_IN_BYTE 8
#define PAGE_4KB_SIZE 4096
#define PAGE_4KB_POWER_OF_2 12
#define PAGE_2MB_SIZE 2097152
#define PAGE_2MB_POWER_OF_2 21
#define PAGE_1GB_SIZE 1073741824
#define PAGE_1GB_POWER_OF_2 30
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
#define PAGE_TABLE_ENTRIES 512



#ifndef __EFI_LOADER__
void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD paging_init(KernelArgs* ka);



void KERNEL_CALL paging_map_page(vaddr_t va,paddr_t pa);



paddr_t KERNEL_CALL paging_get_phys_pml4(void);



paddr_t KERNEL_CALL paging_reverse_translate(vaddr_t va);



void KERNEL_CALL paging_unmap_page(vaddr_t va);



void KERNEL_CALL paging_identity_map(vaddr_t a,uint64_t c);



void KERNEL_CALL paging_identity_unmap(vaddr_t a,uint64_t c);
#endif



#endif
