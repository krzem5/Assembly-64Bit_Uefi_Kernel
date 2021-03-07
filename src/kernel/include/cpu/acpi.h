#ifndef __KERNEL_CPU_ACPI_H__
#define __KERNEL_CPU_ACPI_H__ 1
#include <shared.h>
#include <kmain.h>
#include <stdint.h>



typedef struct __ACPI_DATA{
	uint32_t cpu_c;
	uint8_t cpu[];
} acpi_data_t;



void KERNEL_CALL acpi_init(KernelArgs* ka);



acpi_data_t* KERNEL_CALL acpi_get_data(void);



void KERNEL_CALL acpi_free_data(void);



#endif
