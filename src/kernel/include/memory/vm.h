#ifndef __KERNEL_MEMORY_VM_H__
#define __KERNEL_MEMORY_VM_H__ 1
#include <shared.h>
#include <kmain.h>
#include <stdint.h>



typedef uint64_t vaddr_t;
typedef uint64_t VmMemMapData;
typedef struct __VM_MEM_MAP{
	uint64_t n_va;
	uint64_t b;
	VmMemMapData e[];
} VmMemMap;



extern uint64_t MAX_PROCESS_RAM;



void KERNEL_CALL vm_init(KernelArgs* ka);



void KERNEL_CALL vm_after_pm_init(KernelArgs* ka);



vaddr_t KERNEL_CALL vm_reserve(uint64_t c);



vaddr_t KERNEL_CALL vm_commit(uint64_t c);



vaddr_t KERNEL_CALL vm_commit_2mb(uint64_t c);



vaddr_t KERNEL_CALL vm_current_top(void);



vaddr_t KERNEL_CALL vm_get_top(void);



extern uint64_t KERNEL_CALL asm_get_cr2(void);



#endif
