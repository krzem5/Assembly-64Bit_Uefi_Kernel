#ifndef __KERNEL_MEMORY_PM_H__
#define __KERNEL_MEMORY_PM_H__ 1
#include <shared.h>
#include <kmain.h>
#include <stdint.h>



typedef uint64_t paddr_t;
typedef uint64_t PageData;
typedef struct __PAGE_ARRAY_DATA{
	uint64_t l;
	uint64_t a;
	uint64_t ai;
	uint8_t abi;
	PageData e[];
} PageArrayData;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pm_init(KernelArgs* ka);



paddr_t KERNEL_CALL pm_get_free(void);



void KERNEL_CALL pm_set_free(paddr_t pa);



#endif
