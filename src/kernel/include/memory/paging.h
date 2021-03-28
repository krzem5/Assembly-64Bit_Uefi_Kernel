#ifndef __KERNEL_MEMORY_PAGING_H__
#define __KERNEL_MEMORY_PAGING_H__ 1
#include <shared.h>
#include <kmain.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stdint.h>



#define BITS_IN_BYTE 8
#define PAGE_4KB_SIZE 0x1000
#define PAGE_4KB_POWER_OF_2 12
#define PAGE_2MB_SIZE 2097152
#define PAGE_2MB_POWER_OF_2 21
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



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD paging_init(KernelArgs* ka);



void KERNEL_CALL paging_set_page(vaddr_t va,paddr_t pa);



paddr_t KERNEL_CALL paging_get_phys_pml4(void);



#endif
