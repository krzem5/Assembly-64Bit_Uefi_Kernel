#ifndef __KERNEL_CPU_PORTS_H__
#define __KERNEL_CPU_PORTS_H__ 1
#include <shared.h>
#include <stdint.h>



extern uint8_t KERNEL_CALL asm_port_in8(uint16_t p);



extern uint16_t KERNEL_CALL asm_port_in16(uint16_t p);



extern uint32_t KERNEL_CALL asm_port_in32(uint16_t p);



extern void KERNEL_CALL asm_port_out8(uint16_t p,uint8_t v);



extern void KERNEL_CALL asm_port_out16(uint16_t p,uint16_t v);



extern void KERNEL_CALL asm_port_out32(uint16_t p,uint32_t v);



#endif
