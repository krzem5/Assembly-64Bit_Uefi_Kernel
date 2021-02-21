#ifndef __KERNEL_CPU_ACPI_H__
#define __KERNEL_CPU_ACPI_H__ 1
#include <shared.h>
#include <kmain.h>
#include <stdint.h>



typedef struct __HPET{
	uint64_t b;
} HPET;



void KERNEL_CALL acpi_init(KernelArgs* ka);



#endif
