#ifndef __KERNEL_CPU_APIC_H__
#define __KERNEL_CPU_APIC_H__ 1
#include <shared.h>
#include <stdint.h>



#define IPI_DELIVERY_FIXED 0
#define IPI_DELIVERY_LOWEST_PRIORITY 1
#define IPI_DELIVERY_SMI 2
#define IPI_DELIVERY_NMI 4
#define IPI_DELIVERY_INIT 5
#define IPI_DELIVERY_STARTUP 6
#define IPI_DESTINATION_PHYSICAL 0
#define IPI_DESTINATION_LOGICAL 8
#define IPI_LEVEL_DEASSERT 0
#define IPI_LEVEL_ASSERT 64
#define IPI_TRIGGER_EDGE 0
#define IPI_TRIGGER_LEVEL 128
#define IPI_DESTINATION_SHORTHAND_NONE 0
#define IPI_DESTINATION_SHORTHAND_SELF 1024
#define IPI_DESTINATION_SHORTHAND_ALL_WITH_SELF 2048
#define IPI_DESTINATION_SHORTHAND_ALL_WITHOUT_SELF 3072



void KERNEL_CALL apic_init(uint64_t a);



uint64_t KERNEL_CALL apic_base(void);



void KERNEL_CALL apic_enable(void);



uint8_t KERNEL_CALL apic_get_id(void);



void KERNEL_CALL apic_setup_timer(void);



void KERNEL_CALL apic_send_ipi(uint16_t t,uint8_t v,uint8_t d);



#endif
