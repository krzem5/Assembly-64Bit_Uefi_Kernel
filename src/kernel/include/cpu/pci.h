#ifndef __KERNEL_CPU_PCI_H__
#define __KERNEL_CPU_PCI_H__ 1
#include <shared.h>
#include <stdint.h>



#define PCI_VENDOR_REGISTER 0
#define PCI_STATUS_COMMAND_REGISTER 0x04
#define PCI_ID_REGISTER 0x08
#define PCI_MISC_REGISTER 0x0c
#define PCI_BAR0_REGISTER 0x10
#define PCI_BAR1_REGISTER 0x14
#define PCI_BAR2_REGISTER 0x18
#define PCI_BAR3_REGISTER 0x1c
#define PCI_BAR4_REGISTER 0x20
#define PCI_BAR5_REGISTER 0x24
#define PCI_COMMAND_IO 1
#define PCI_COMMAND_MEMORY_SPACE 2
#define PCI_COMMAND_MASTER 4
#define PCI_COMMAND_SPECIAL_CYCLES 8
#define PCI_COMMAND_MEMORY_WRITE_INVALIDATE 16
#define PCI_COMMAND_VGA_PALETTE 32
#define PCI_COMMAND_PARITY_ERROR 64
#define PCI_COMMAND_SERR 256
#define PCI_COMMAND_FAST_BACK_TO_BACK 512
#define PCI_COMMAND_DISABLE_INTERRUPT 1024



typedef struct __PCI_DEVICE{
	uint32_t port;
	uint8_t bus;
	uint8_t dev_func;
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t revision;
	uint8_t interface;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t type;
	uint8_t bist;
	const char* class_code_str;
	const char* subclass_str;
	void* drv_dt;
} pci_device_t;



typedef struct __PCI_DEVICE_LIST{
	uint32_t l;
	pci_device_t* dt;
} pci_device_list_t;



extern pci_device_list_t pci_list;



extern uint32_t KERNEL_CALL asm_pci_read(uint32_t p);



extern void KERNEL_CALL asm_pci_write(uint32_t p,uint32_t v);



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pci_init(void);



#endif
