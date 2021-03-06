#include <shared.h>
#include <cpu/cpu.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <stdint.h>



cpu_t _cpu_dt;



void KERNEL_CALL wrmsr(uint64_t a,uint64_t v){
	__asm__ volatile("wrmsr"::"a"(v&0xffffffff),"c"(a),"d"(v>>32));
}



void KERNEL_CALL cpu_init(uint64_t apic_a){
	_cpu_dt.s=&_cpu_dt;
	_cpu_dt.id=(uint8_t)(*((uint32_t*)apic_a+4)>>24);
	_cpu_dt.idx=0;
	_cpu_dt.apic_a=apic_a;
	wrmsr(0xc0000101,(uint64_t)(void*)&_cpu_dt);
	ASSERT(current_cpu()==&_cpu_dt);
}
