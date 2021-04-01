#ifndef __KERNEL_DRIVER_ATA_H__
#define __KERNEL_DRIVER_ATA_H__ 1
#include <shared.h>
#include <cpu/pci.h>
#include <memory/pm.h>
#include <memory/vm.h>
#include <stdint.h>



#define ATA_DRIVER_DATA_REGISTER(c)  ((c).base)
#define ATA_DRIVER_ERROR_REGISTER(c) ((c).base+1)
#define ATA_DRIVER_FEATURES_REGISTER 1
#define ATA_DRIVER_SECTOR_COUNT0_REGISTER(c) ((c).base+2)
#define ATA_DRIVER_LBA0_REGISTER(c) ((c).base+3)
#define ATA_DRIVER_LBA1_REGISTER(c) ((c).base+4)
#define ATA_DRIVER_LBA2_REGISTER(c) ((c).base+5)
#define ATA_DRIVER_SELECT_DRIVE_REGISTER(c) ((c).base+6)
#define ATA_DRIVER_COMMAND_REGISTER(c) ((c).base+7)
#define ATA_DRIVER_STATUS_REGISTER(c) ((c).base+7)
#define ATA_DRIVER_SECTOR_COUNT1_REGISTER(c) ((c).base+2)
#define ATA_DRIVER_LBA3_REGISTER(c) ((c).base+3)
#define ATA_DRIVER_LBA4_REGISTER(c) ((c).base+4)
#define ATA_DRIVER_LBA5_REGISTER(c) ((c).base+5)
#define ATA_DRIVER_CONTROL_REGISTER(c) ((c).ctrl+2)
#define ATA_DRIVER_BUS_MASTERING_COMMAND_REGISTER(c) ((c).bmide)
#define ATA_DRIVER_BUS_MASTERING_STATUS_REGISTER(c) ((c).bmide+2)
#define ATA_DRIVER_BUS_MASTERING_POINTER_REGISTER(c) ((c).bmide+4)
#define ATA_DRIVER_BUS_MASTERING_COMMAND_DMA_STOP 0x0
#define ATA_DRIVER_BUS_MASTERING_COMMAND_DMA_START 0x1
#define ATA_DRIVER_BUS_MASTERING_COMMAND_READ 0x8
#define ATA_DRIVER_BUS_MASTERING_STATUS_ERROR 0x2
#define ATA_DRIVER_BUS_MASTERING_STATUS_INTERRUPT 0x4
#define ATA_DRIVER_SELECT_DRIVE_SET_DRIVER(d) ((d)<<4)
#define ATA_DRIVER_SELECT_DRIVE_CHS_FLAG 0x00
#define ATA_DRIVER_SELECT_DRIVE_LBA_FLAG 0x40
#define ATA_DRIVER_SELECT_DRIVE_OBSOLETE_FLAG 0xa0
#define ATA_DRIVER_COMMAND_READ_PIO 0x20
#define ATA_DRIVER_COMMAND_READ_PIO_EXT 0x24
#define ATA_DRIVER_COMMAND_READ_DMA 0xc8
#define ATA_DRIVER_COMMAND_READ_DMA_EXT 0x25
#define ATA_DRIVER_COMMAND_WRITE_PIO 0x30
#define ATA_DRIVER_COMMAND_WRITE_PIO_EXT 0x34
#define ATA_DRIVER_COMMAND_WRITE_DMA 0xca
#define ATA_DRIVER_COMMAND_WRITE_DMA_EXT 0x35
#define ATA_DRIVER_COMMAND_CACHE_FLUSH 0xe7
#define ATA_DRIVER_COMMAND_CACHE_FLUSH_EXT 0xca
#define ATA_DRIVER_COMMAND_PACKET 0xa0
#define ATA_DRIVER_COMMAND_IDENTIFY_PACKET 0xa1
#define ATA_DRIVER_COMMAND_IDENTIFY 0xec
#define ATA_DRIVER_STATUS_ERROR 0x01
#define ATA_DRIVER_STATUS_INDEX 0x02
#define ATA_DRIVER_STATUS_DATA_CORRECTED 0x04
#define ATA_DRIVER_STATUS_REQUEST_READY 0x08
#define ATA_DRIVER_STATUS_SEEK_COMPLETE 0x10
#define ATA_DRIVER_STATUS_FAULT 0x20
#define ATA_DRIVER_STATUS_READY 0x40
#define ATA_DRIVER_STATUS_BUSY 0x80
#define ATA_DRIVER_CONTROL_ZERO 0x00
#define ATA_DRIVER_CONTROL_DISABLE_INTERRUPTS 0x02
#define ATA_DRIVER_CONTROL_SOFTWARE_RESET 0x04
#define ATA_DRIVER_CONTROL_ENABLE_HIGH_FUNCTIONS 0x80
#define ATA_DRIVER_IDENTIFY_DEVICE_TYPE_OFFSET 0
#define ATA_DRIVER_IDENTIFY_CYLINDERS_OFFSET 2
#define ATA_DRIVER_IDENTIFY_HEADS_OFFSET 6
#define ATA_DRIVER_IDENTIFY_SECTORS_OFFSET 12
#define ATA_DRIVER_IDENTIFY_SERIAL_OFFSET 20
#define ATA_DRIVER_IDENTIFY_MODEL_OFFSET 54
#define ATA_DRIVER_IDENTIFY_CAPABILITIES_OFFSET 98
#define ATA_DRIVER_IDENTIFY_FIELD_VALID_OFFSET 106
#define ATA_DRIVER_IDENTIFY_MAX_LBA_OFFSET 120
#define ATA_DRIVER_IDENTIFY_COMMAND_SET_OFFSET 164
#define ATA_DRIVER_IDENTIFY_MAX_LBA_EXT_OFFSET 200
#define ATA_MAX_DEVICES 16
#define ATA_SECTOR_SIZE 512
#define ATA_DEVICE_TYPE_ATA 0
#define ATA_DEVICE_TYPE_ATAPI 1
#define ATA_DEVICE_GET_EXISTS(d) ((d).f&1)
#define ATA_DEVICE_GET_CHANNEL(d) (((d).f>>1)&1)
#define ATA_DEVICE_GET_DRIVE(d) (((d).f>>2)&1)
#define ATA_DEVICE_GET_TYPE(d) (((d).f>>3)&1)
#define ATA_DEVICE_SET_EXISTS 1
#define ATA_DEVICE_SET_CHANNEL(c) ((c)<<1)
#define ATA_DEVICE_SET_DRIVE(d) ((d)<<2)
#define ATA_DEVICE_SET_TYPE(t) ((t)<<3)
#define ATA_DEVICE_LIST_GET_LENGTH(l) ((l).f&15)
#define ATA_DEVICE_LIST_GET_FREE_INDEX(l) ((l).f>>4)
#define ATA_DEVICE_LIST_SET_LENGTH(l) (l)
#define ATA_DEVICE_LIST_SET_FREE_INDEX(i) ((i)<<4)
#define ATA_DEVICE_LIST_INC_LENGTH(l) (((l).f&15)+1)
#define ATA_ERROR_OK 0
#define ATA_ERROR_NO_DEVICE 1
#define ATA_ERROR_INVALID_POSITION 2



typedef struct __IDE_CONTROLLER_CHANNEL{
	uint16_t base;
	uint16_t ctrl;
	uint16_t bmide;
	uint8_t int_;
} ide_controller_channel_t;



typedef struct __ATA_DEVICE{
	ide_controller_channel_t ch;
	uint8_t f;
	char nm[41];
	uint16_t sig;
	uint16_t cap;
	uint32_t cmd_set;
	uint64_t sz;
	paddr_t pa;
	vaddr_t va;
} ata_device_t;



typedef struct __ATA_DEVICE_LIST{
	uint8_t f;
	ata_device_t dt[ATA_MAX_DEVICES];
} ata_device_list_t;



extern uint8_t KERNEL_CALL asm_ata_read(ide_controller_channel_t* c,uint8_t r);



extern void KERNEL_CALL asm_ata_write(ide_controller_channel_t* c,uint8_t r,uint8_t v);



void KERNEL_CALL ata_init(pci_device_t* pci);



uint8_t KERNEL_CALL ata_read_sectors(ata_device_t* dv,uint64_t lba,uint16_t c);



uint8_t KERNEL_CALL ata_write_sectors(ata_device_t* dv,uint64_t lba,uint16_t c);



#endif
