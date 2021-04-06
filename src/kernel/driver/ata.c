#include <shared.h>
#include <cpu/hpet_timer.h>
#include <cpu/pci.h>
#include <cpu/ports.h>
#include <driver/ata.h>
#include <gfx/console.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stddef.h>
#include <stdint.h>



#define IO_WAIT(c) (asm_port_in8(ATA_DRIVER_ALT_STATUS_REGISTER(c)),asm_port_in8(ATA_DRIVER_ALT_STATUS_REGISTER(c)),asm_port_in8(ATA_DRIVER_ALT_STATUS_REGISTER(c)),asm_port_in8(ATA_DRIVER_ALT_STATUS_REGISTER(c)))



uint8_t _bf[512];
ata_device_list_t dl={
	ATA_DEVICE_LIST_SET_LENGTH(0)|ATA_DEVICE_LIST_SET_FREE_INDEX(0)
};
uint8_t _dl_init=0;



void KERNEL_CALL ata_init(pci_device_t* pci){
	if (!_dl_init){
		_dl_init=1;
		for (uint8_t i=0;i<ATA_MAX_DEVICES;i++){
			dl.dt[i].f=0;
		}
	}
	uint32_t b0=asm_pci_read(pci->port|PCI_BAR0_REGISTER);
	uint32_t b1=asm_pci_read(pci->port|PCI_BAR1_REGISTER);
	uint32_t b2=asm_pci_read(pci->port|PCI_BAR2_REGISTER);
	uint32_t b3=asm_pci_read(pci->port|PCI_BAR3_REGISTER);
	uint32_t b4=asm_pci_read(pci->port|PCI_BAR4_REGISTER);
	uint32_t cmd=asm_pci_read(pci->port|PCI_STATUS_COMMAND_REGISTER);
	if (!(cmd&PCI_COMMAND_MASTER)){
		cmd|=PCI_COMMAND_MASTER;
		asm_pci_write(pci->port|PCI_STATUS_COMMAND_REGISTER,cmd);
	}
	if (!(cmd&PCI_COMMAND_IO)){
		cmd|=PCI_COMMAND_IO;
		asm_pci_write(pci->port|PCI_STATUS_COMMAND_REGISTER,cmd);
	}
	if (!(cmd&PCI_COMMAND_DISABLE_INTERRUPT)){
		cmd&=~PCI_COMMAND_DISABLE_INTERRUPT;
		asm_pci_write(pci->port|PCI_STATUS_COMMAND_REGISTER,cmd);
	}
	for (uint8_t i=0;i<2;i++){
		ide_controller_channel_t c={
			(!i?b0:b2)&0xfffffffc,
			(!i?b1:b3)&0xfffffffc,
			b4&0xfffffffc
		};
		if (!c.base){
			c.base=0x170+(!i?0x80:0);
		}
		if (!c.ctrl){
			c.ctrl=0x376+(!i?0x80:0);
		}
		if (i){
			c.bmide+=8;
		}
		for (uint8_t j=0;j<2;j++){
			asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(c),ATA_DRIVER_CONTROL_SOFTWARE_RESET|ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
			IO_WAIT(c);
			asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(c),ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
			IO_WAIT(c);
			asm_port_out8(ATA_DRIVER_SELECT_DRIVE_REGISTER(c),ATA_DRIVER_SELECT_DRIVE_OBSOLETE_FLAG|ATA_DRIVER_SELECT_DRIVE_SET_DRIVE(j));
			asm_port_out8(ATA_DRIVER_SECTOR_COUNT0_REGISTER(c),0);
			asm_port_out8(ATA_DRIVER_LBA0_REGISTER(c),0);
			asm_port_out8(ATA_DRIVER_LBA1_REGISTER(c),0);
			asm_port_out8(ATA_DRIVER_LBA2_REGISTER(c),0);
			asm_port_out8(ATA_DRIVER_COMMAND_REGISTER(c),ATA_DRIVER_COMMAND_IDENTIFY);
			IO_WAIT(c);
			uint8_t s=asm_port_in8(ATA_DRIVER_STATUS_REGISTER(c));
			if (!s){
				continue;
			}
			uint8_t err=0;
			uint8_t t=ATA_DEVICE_TYPE_ATA;
			while (!err){
				if ((s&ATA_DRIVER_STATUS_ERROR)||(s&ATA_DRIVER_STATUS_FAULT)){
					uint8_t lba1=asm_port_in8(ATA_DRIVER_LBA1_REGISTER(c));
					uint8_t lba2=asm_port_in8(ATA_DRIVER_LBA2_REGISTER(c));
					if ((lba1==0x14&&lba2==0xeb)||(lba1==0x69&&lba2==0x96)){
						t=ATA_DEVICE_TYPE_ATAPI;
						asm_port_out8(ATA_DRIVER_COMMAND_REGISTER(c),ATA_DRIVER_COMMAND_IDENTIFY);
						IO_WAIT(c);
						break;
					}
					err=1;
					break;
				}
				if (!(s&ATA_DRIVER_STATUS_BUSY)&&(s&ATA_DRIVER_STATUS_REQUEST_READY)){
					break;
				}
				s=asm_port_in8(ATA_DRIVER_STATUS_REGISTER(c));
			}
			if (err){
				continue;
			}
			for (uint16_t k=0;k<256;k++){
				*((uint16_t*)_bf+k)=asm_port_in16(ATA_DRIVER_DATA_REGISTER(c));
			}
			ata_device_t* dv=dl.dt+ATA_DEVICE_LIST_GET_FREE_INDEX(dl);
			uint8_t k=ATA_DEVICE_LIST_GET_FREE_INDEX(dl);
			for (;k<ATA_MAX_DEVICES;k++){
				if (!ATA_DEVICE_GET_EXISTS(dl.dt[k])){
					break;
				}
			}
			dl.f=ATA_DEVICE_LIST_INC_LENGTH(dl)|ATA_DEVICE_LIST_SET_FREE_INDEX(k);
			dv->ch=c;
			dv->f=ATA_DEVICE_SET_EXISTS|ATA_DEVICE_SET_DRIVE(j)|ATA_DEVICE_SET_TYPE(t);
			for (uint8_t k=0;k<40;k+=2){
				dv->nm[k]=_bf[ATA_DRIVER_IDENTIFY_MODEL_OFFSET+k+1];
				dv->nm[k+1]=_bf[ATA_DRIVER_IDENTIFY_MODEL_OFFSET+k];
			}
			dv->nm[40]=0;
			dv->sig=*((uint16_t*)(_bf+ATA_DRIVER_IDENTIFY_DEVICE_TYPE_OFFSET));
			dv->cap=*((uint16_t*)(_bf+ATA_DRIVER_IDENTIFY_CAPABILITIES_OFFSET));
			dv->cmd_set=*((uint32_t*)(_bf+ATA_DRIVER_IDENTIFY_COMMAND_SET_OFFSET));
			dv->sz=(*((uint64_t*)(_bf+(dv->cmd_set&0x4000000?ATA_DRIVER_IDENTIFY_MAX_LBA_EXT_OFFSET:ATA_DRIVER_IDENTIFY_MAX_LBA_OFFSET))))*ATA_SECTOR_SIZE;
			dv->pa=pm_get_free();
			dv->va=vm_commit_fixed(dv->pa);
			for (uint16_t k=0;k<512;k++){
				*((uint64_t*)dv->va+k)=0;
			}
			console_log("ATA Device:\n  Flags:        %.2hhx\n  Name:         %s\n  Signature:    %.4hx\n  Capabilities: %.4x\n  Command Set:  %.8x\n  Size:         %lu (%u.%.2u Gb)\n  Buffer PA:   %p\n  Buffer VA:   %p\n",dv->f,dv->nm,dv->sig,dv->cap,dv->cmd_set,dv->sz,dv->sz/1073741824,(dv->sz/10737418)%100,dv->pa,dv->va);
		}
	}
}



void KERNEL_CALL ata_test_drives(void){
	for (uint8_t k=0;k<ATA_MAX_DEVICES;k++){
		if (ATA_DEVICE_GET_EXISTS(dl.dt[k])){
			console_log("%.2hhx\n",dl.dt[k].f);
		}
	}
}



uint8_t KERNEL_CALL ata_read_sectors(ata_device_t* dv,uint64_t lba,uint16_t c){
	if (!ATA_DEVICE_GET_EXISTS(*dv)){
		return ATA_ERROR_NO_DEVICE;
	}
	if (ATA_DEVICE_GET_TYPE(*dv)==ATA_DEVICE_TYPE_ATA&&lba+c>dv->sz){
		return ATA_ERROR_INVALID_POSITION;
	}
	if (!c){
		return ATA_ERROR_OK;
	}
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_COMMAND_REGISTER(dv->ch),0);
	asm_port_out32(ATA_DRIVER_BUS_MASTERING_POINTER_REGISTER(dv->ch),(uint32_t)(dv->pa)|(asm_port_in32(ATA_DRIVER_BUS_MASTERING_POINTER_REGISTER(dv->ch))&3));
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch),asm_port_in8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch))|ATA_DRIVER_BUS_MASTERING_STATUS_INTERRUPT|ATA_DRIVER_BUS_MASTERING_STATUS_ERROR);
	asm_port_out8(ATA_DRIVER_SELECT_DRIVE_REGISTER(dv->ch),ATA_DRIVER_SELECT_DRIVE_OBSOLETE_FLAG|ATA_DRIVER_SELECT_DRIVE_LBA_FLAG|ATA_DRIVER_SELECT_DRIVE_SET_DRIVE(ATA_DEVICE_GET_DRIVE(*dv)));
	asm_port_out8(ATA_DRIVER_SECTOR_COUNT0_REGISTER(dv->ch),c&0xff);
	asm_port_out8(ATA_DRIVER_LBA0_REGISTER(dv->ch),lba&0xff);
	asm_port_out8(ATA_DRIVER_LBA1_REGISTER(dv->ch),(lba>>8)&0xff);
	asm_port_out8(ATA_DRIVER_LBA2_REGISTER(dv->ch),(lba>>16)&0xff);
	asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(dv->ch),ATA_DRIVER_CONTROL_ENABLE_HIGH_FUNCTIONS|ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
	asm_port_out8(ATA_DRIVER_SECTOR_COUNT1_REGISTER(dv->ch),c>>8);
	asm_port_out8(ATA_DRIVER_LBA3_REGISTER(dv->ch),(lba>>24)&0xff);
	asm_port_out8(ATA_DRIVER_LBA4_REGISTER(dv->ch),(lba>>32)&0xff);
	asm_port_out8(ATA_DRIVER_LBA5_REGISTER(dv->ch),(lba>>40)&0xff);
	asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(dv->ch),ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
	asm_port_out8(ATA_DRIVER_COMMAND_REGISTER(dv->ch),ATA_DRIVER_COMMAND_READ_DMA);
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_COMMAND_REGISTER(dv->ch),ATA_DRIVER_BUS_MASTERING_COMMAND_READ|ATA_DRIVER_BUS_MASTERING_COMMAND_DMA_START);
	while (1){
		uint8_t s=asm_port_in8(ATA_DRIVER_STATUS_REGISTER(dv->ch));
		if (s&ATA_DRIVER_STATUS_ERROR){
			return ATA_ERROR_INTERNAl_ERROR;
		}
		if (!(asm_port_in8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch))&ATA_DRIVER_BUS_MASTERING_STATUS_INTERRUPT)){
			continue;
		}
		if (!(s&ATA_DRIVER_STATUS_BUSY)){
			break;
		}
	}
	return ATA_ERROR_OK;
}



uint8_t KERNEL_CALL ata_write_sectors(ata_device_t* dv,uint64_t lba,uint16_t c){
	if (!ATA_DEVICE_GET_EXISTS(*dv)){
		return ATA_ERROR_NO_DEVICE;
	}
	if (ATA_DEVICE_GET_TYPE(*dv)==ATA_DEVICE_TYPE_ATA&&lba+c>dv->sz){
		return ATA_ERROR_INVALID_POSITION;
	}
	if (!c){
		return ATA_ERROR_OK;
	}
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_COMMAND_REGISTER(dv->ch),0);
	asm_port_out32(ATA_DRIVER_BUS_MASTERING_POINTER_REGISTER(dv->ch),(uint32_t)(dv->pa)|(asm_port_in32(ATA_DRIVER_BUS_MASTERING_POINTER_REGISTER(dv->ch))&3));
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch),asm_port_in8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch))|ATA_DRIVER_BUS_MASTERING_STATUS_INTERRUPT|ATA_DRIVER_BUS_MASTERING_STATUS_ERROR);
	asm_port_out8(ATA_DRIVER_SELECT_DRIVE_REGISTER(dv->ch),ATA_DRIVER_SELECT_DRIVE_OBSOLETE_FLAG|ATA_DRIVER_SELECT_DRIVE_LBA_FLAG|ATA_DRIVER_SELECT_DRIVE_SET_DRIVE(ATA_DEVICE_GET_DRIVE(*dv)));
	asm_port_out8(ATA_DRIVER_SECTOR_COUNT0_REGISTER(dv->ch),c&0xff);
	asm_port_out8(ATA_DRIVER_LBA0_REGISTER(dv->ch),lba&0xff);
	asm_port_out8(ATA_DRIVER_LBA1_REGISTER(dv->ch),(lba>>8)&0xff);
	asm_port_out8(ATA_DRIVER_LBA2_REGISTER(dv->ch),(lba>>16)&0xff);
	asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(dv->ch),ATA_DRIVER_CONTROL_ENABLE_HIGH_FUNCTIONS|ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
	asm_port_out8(ATA_DRIVER_SECTOR_COUNT1_REGISTER(dv->ch),c>>8);
	asm_port_out8(ATA_DRIVER_LBA3_REGISTER(dv->ch),(lba>>24)&0xff);
	asm_port_out8(ATA_DRIVER_LBA4_REGISTER(dv->ch),(lba>>32)&0xff);
	asm_port_out8(ATA_DRIVER_LBA5_REGISTER(dv->ch),(lba>>40)&0xff);
	asm_port_out8(ATA_DRIVER_CONTROL_REGISTER(dv->ch),ATA_DRIVER_CONTROL_OBSOLETE|ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS);
	asm_port_out8(ATA_DRIVER_COMMAND_REGISTER(dv->ch),ATA_DRIVER_COMMAND_WRITE_DMA);
	asm_port_out8(ATA_DRIVER_BUS_MASTERING_COMMAND_REGISTER(dv->ch),ATA_DRIVER_BUS_MASTERING_COMMAND_DMA_START);
	while (1){
		uint8_t s=asm_port_in8(ATA_DRIVER_STATUS_REGISTER(dv->ch));
		if (s&ATA_DRIVER_STATUS_ERROR){
			return ATA_ERROR_INTERNAl_ERROR;
		}
		if (!(asm_port_in8(ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(dv->ch))&ATA_DRIVER_BUS_MASTERING_STATUS_INTERRUPT)){
			continue;
		}
		if (!(s&ATA_DRIVER_STATUS_BUSY)){
			break;
		}
	}
	return ATA_ERROR_OK;
}
