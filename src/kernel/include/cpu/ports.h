#ifndef __KERNEL_CPU_PORTS_H__
#define __KERNEL_CPU_PORTS_H__ 1
#include <shared.h>
#include <stdint.h>



uint8_t KERNEL_CALL port_in(uint16_t p);



void KERNEL_CALL port_out(uint16_t p,uint8_t v);



#endif
