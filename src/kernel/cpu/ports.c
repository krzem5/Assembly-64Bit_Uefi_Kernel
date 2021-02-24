#include <shared.h>
#include <cpu/ports.h>
#include <stdint.h>



uint8_t KERNEL_CALL port_in(uint16_t p){
	uint8_t o;
	__asm__ volatile("inb %1, %0":"=a"(o):"dN"(p));
	return o;
}



void KERNEL_CALL port_out(uint16_t p,uint8_t v){
	__asm__ volatile("outb %1, %0"::"dN"(p),"a"(v));
}
