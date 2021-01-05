#ifndef __KERNEL_CPU_PORTS_H__
#define __KERNEL_CPU_PORTS_H__ 1
#include <libc/stdint.h>



uint8_t port_in(uint16_t p);



void port_out(uint16_t p,uint8_t v);



#endif
