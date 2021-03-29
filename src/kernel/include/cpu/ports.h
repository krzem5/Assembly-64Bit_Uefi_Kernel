#ifndef __KERNEL_CPU_PORTS_H__
#define __KERNEL_CPU_PORTS_H__ 1
#include <shared.h>
#include <stdint.h>



extern uint8_t KERNEL_CALL asm_port_in(uint16_t p);



extern void KERNEL_CALL asm_port_out(uint16_t p,uint8_t v);



#endif
