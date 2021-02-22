#include <shared.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define PROCESS_TIME_MS 1



uint64_t _tm_n_sw=0;



void KERNEL_CALL scheduler_init(void){
	_tm_n_sw=0;
}



void KERNEL_CALL scheduler_start(void){
	for (;;){
		__asm__("hlt");
	}
}



void KERNEL_CALL scheduler_tick(uint64_t tm){
	if (tm<=_tm_n_sw){
		return;
	}
	_tm_n_sw=tm+PROCESS_TIME_MS;
	console_log("Reschedule!\n");
}
