#include <shared.h>
#include <cpu/hpet_timer.h>
#include <cpu/pci.h>
#include <driver/ide.h>
#include <gfx/console.h>
#include <stdint.h>



void KERNEL_CALL ide_init(pci_device_t* pci){
	uint32_t b0=asm_pci_read(pci->port|PCI_BAR0_REGISTER);
	uint32_t b1=asm_pci_read(pci->port|PCI_BAR1_REGISTER);
	uint32_t b2=asm_pci_read(pci->port|PCI_BAR2_REGISTER);
	uint32_t b3=asm_pci_read(pci->port|PCI_BAR3_REGISTER);
	uint32_t b4=asm_pci_read(pci->port|PCI_BAR4_REGISTER);
	ide_device_t d={
		{
			{
				(b0?b0&0xfffffffc:0x1f0),
				(b1?b1&0xfffffffc:0x3f6),
				b4&0xfffffffc,
				1
			},
			{
				(b2?b2&0xfffffffc:0x170),
				(b3?b3&0xfffffffc:0x376),
				(b4&0xfffffffc)+8,
				1
			}
		}
	};
	asm_ide_write(&d.c[0],IDE_CONTROL_REGISTER,2);
	asm_ide_write(&d.c[1],IDE_CONTROL_REGISTER,2);
	for (uint8_t i=0;i<2;i++){
		for (uint8_t j=0;j<2;j++){
			asm_ide_write(&d.c[i],IDE_HDDEVSEL_REGISTER,0xa0|(j<<4));
			hpet_timer_spinwait(1000);
			asm_ide_write(&d.c[i],IDE_COMMAND_REGISTER,IDE_COMMAND_IDENTIFY);
			hpet_timer_spinwait(1000);
			if (!asm_ide_read(&d.c[i],IDE_STATUS_REGISTER)){
				console_log("Failed!\n");
				continue;
			}
			console_ok("Succeded: %u %u\n",i,j);
		}
	}
	for (;;);
}
