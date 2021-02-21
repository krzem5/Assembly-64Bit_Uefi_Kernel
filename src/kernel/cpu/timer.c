#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/timer.h>
#include <gfx/console.h>



void KERNEL_CALL timer_init(HPET hpet){
	console_log("HPET Base Ptr: %p\n",hpet.b);
	uint64_t* ptr=(uint64_t*)(void*)hpet.b;
	*(ptr+2)=0;
	*(ptr+30)=0;
	console_log("%llu, %llu (%llu)\n",(((*ptr)>>8)&0x1f)+1,(*ptr)>>32,1000000000/((*ptr)>>32));
}
