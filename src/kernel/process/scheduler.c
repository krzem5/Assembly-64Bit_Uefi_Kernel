#include <shared.h>
#include <cpu/timer.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define PROCESS_TIME_US 1000000ull



uint64_t _tm_n_sw=0;



void KERNEL_CALL scheduler_init(void){
	timer_set_frequency(PROCESS_TIME_US);
}



void KERNEL_CALL KERNEL_NO_RETURN scheduler_start(void){
	__asm__("sti");
	for (;;){
		__asm__("hlt");
	}
}



void KERNEL_CALL scheduler_tick(uint64_t tm){
	if (tm<=_tm_n_sw){
		return;
	}
	_tm_n_sw=tm+PROCESS_TIME_US;
	console_log("Reschedule! (%llu)\n",tm);
}
