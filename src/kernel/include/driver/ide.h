#include <shared.h>
#include <cpu/pci.h>
#include <stdint.h>



#define IDE_DATA_REGISTER 0
#define IDE_ERROR_REGISTER 1
#define IDE_FEATURES_REGISTER 1
#define IDE_SECCOUNT0_REGISTER 2
#define IDE_LBA0_REGISTER 3
#define IDE_LBA1_REGISTER 4
#define IDE_LBA2_REGISTER 5
#define IDE_HDDEVSEL_REGISTER 6
#define IDE_COMMAND_REGISTER 7
#define IDE_STATUS_REGISTER 7
#define IDE_SECCOUNT1_REGISTER 8
#define IDE_LBA3_REGISTER 9
#define IDE_LBA4_REGISTER 10
#define IDE_LBA5_REGISTER 11
#define IDE_CONTROL_REGISTER 12
#define IDE_ALTSTATUS_REGISTER 12
#define IDE_COMMAND_READ_PIO 0x20
#define IDE_COMMAND_READ_PIO_EXT 0x24
#define IDE_COMMAND_READ_DMA 0xc8
#define IDE_COMMAND_READ_DMA_EXT 0x25
#define IDE_COMMAND_WRITE_PIO 0x30
#define IDE_COMMAND_WRITE_PIO_EXT 0x34
#define IDE_COMMAND_WRITE_DMA 0xca
#define IDE_COMMAND_WRITE_DMA_EXT 0x35
#define IDE_COMMAND_CACHE_FLUSH 0xe7
#define IDE_COMMAND_CACHE_FLUSH_EXT 0xca
#define IDE_COMMAND_PACKET 0xa0
#define IDE_COMMAND_IDENTIFY_PACKET 0xa1
#define IDE_COMMAND_IDENTIFY 0xec



typedef struct __IDE_DEVICE_CHANNEL{
	uint16_t base;
	uint16_t ctrl;
	uint16_t bmide;
	uint8_t int_;
} ide_device_channel_t;



typedef struct __IDE_DEVICE{
	ide_device_channel_t c[2];
} ide_device_t;



extern uint8_t KERNEL_CALL asm_ide_read(ide_device_channel_t* c,uint8_t r);



extern void KERNEL_CALL asm_ide_write(ide_device_channel_t* c,uint8_t r,uint8_t v);



void KERNEL_CALL ide_init(pci_device_t* pci);
