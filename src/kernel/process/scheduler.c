#include <shared.h>
#include <cpu/hpet_timer.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define PROCESS_TIME_US 1000ull



uint64_t _l_tm=0;



void KERNEL_CALL scheduler_init(void){
	hpet_timer_set_frequency(PROCESS_TIME_US);
}



void KERNEL_CALL KERNEL_NO_RETURN scheduler_start(void){
	__asm__("sti");
	for (;;){
		__asm__("hlt");
	}
}



void KERNEL_CALL scheduler_tick(uint64_t tm){
	console_log("Reschedule: %lluus\n",tm-_l_tm);
	_l_tm=tm;
}
