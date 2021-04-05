#include <shared.h>
#include <cpu/pci.h>
#include <cpu/ports.h>
#include <driver/ata.h>
#include <driver/driver_list.h>
#include <gfx/console.h>
#include <stdint.h>



void KERNEL_CALL driver_list_load(pci_device_t* pci){
	if (pci->class_code==1&&pci->subclass==1&&pci->type==0){
		ata_init(pci);
	}
}
