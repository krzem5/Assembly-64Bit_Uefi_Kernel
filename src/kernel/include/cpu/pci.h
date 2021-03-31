#ifndef __KERNEL_CPU_PCI_H__
#define __KERNEL_CPU_PCI_H__ 1
#include <shared.h>
#include <stdint.h>




typedef struct __PCI_DEVICE_TYPE0{
	uint32_t bar0;
	uint32_t bar1;
	uint32_t bar2;
	uint32_t bar3;
	uint32_t bar4;
	uint32_t bar5;
	uint16_t subsystem_vendor_id;
	uint32_t cardbus_cis_pointer;
	uint32_t expansion_rom_addr;
	uint8_t capablities;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint8_t min_grant;
	uint8_t max_latency;
} pci_device_type0_t;



typedef struct __PCI_DEVICE_TYPE1{
	uint32_t bar0;
	uint32_t bar1;
	uint8_t primary_bus_number;
	uint8_t	secondary_bus_number;
	uint8_t subordinate_bus_number;
	uint8_t second_latency_timer;
	uint32_t io_base;
	uint32_t io_limit;
	uint16_t secondary_status;
	uint16_t mem_base;
	uint16_t mem_limit;
	uint16_t prefetchable_mem_base;
	uint16_t prefetchable_mem_limit;
	uint64_t prefetchable_base;
	uint8_t capablities;
	uint32_t expansion_rom_addr;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;
} pci_device_type1_t;



typedef struct __PCI_DEVICE_TYPE2{
	uint32_t ex_ca_base_addr;
	uint8_t capablities;
	uint16_t secondary_status;
	uint8_t	pci_bus_number;
	uint8_t cb_number;
	uint8_t subordinate_bus_number;
	uint8_t cb_latency_timer;
	uint32_t mem_base_addr0;
	uint32_t mem_limit0;
	uint32_t mem_base_addr1;
	uint32_t mem_limit1;
	uint32_t io_base_addr0;
	uint32_t io_limit0;
	uint32_t io_base_addr1;
	uint32_t io_limit1;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint16_t bridge_control;
	uint16_t subsystem_device_id;
	uint16_t subsystem_vendor_id;
	uint32_t legacy_base_addr;
} pci_device_type2_t;



typedef union __PCI_DEVICE_TYPE{
	pci_device_type0_t t0;
	pci_device_type1_t t1;
	pci_device_type2_t t2;
} pci_device_type_t;



typedef struct __PCI_DEVICE{
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision;
	uint8_t interface;
	uint8_t subclass;
	uint8_t class_;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t type;
	uint8_t bist;
	const char* class_str;
	const char* subclass_str;
	const char* interface_str;
	pci_device_type_t dt;
} pci_device_t;



typedef struct __PCI_DEVICE_LIST{
	uint32_t l;
	pci_device_t* dt;
} pci_device_list_t;



extern pci_device_list_t pci_list;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pci_init(void);



#endif
