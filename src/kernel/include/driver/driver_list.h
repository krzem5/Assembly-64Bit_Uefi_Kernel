#ifndef __KERNEL_DRIVER_DRIVER_LIST_H__
#define __KERNEL_DRIVER_DRIVER_LIST_H__ 1
#include <shared.h>
#include <cpu/pci.h>



void KERNEL_CALL driver_list_load(pci_device_t* pci);



#endif
