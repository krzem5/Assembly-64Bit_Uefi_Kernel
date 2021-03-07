#ifndef __KERNEL_MEMORY_PAGING_H__
#define __KERNEL_MEMORY_PAGING_H__ 1
#include <shared.h>
#include <kmain.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stdint.h>



void KERNEL_CALL paging_init(KernelArgs* ka);



void KERNEL_CALL paging_set_page(vaddr_t va,paddr_t pa);



paddr_t KERNEL_CALL paging_get_phys_pml4(void);



#endif
