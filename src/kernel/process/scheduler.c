#include <shared.h>
#include <cpu/apic.h>
#include <cpu/cpu.h>
#include <cpu/hpet_timer.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define PROCESS_TIME_US 1000ull



uint64_t _l_tm;



void KERNEL_CALL scheduler_init(void){

}



void KERNEL_CALL scheduler_start(void){
	_l_tm=hpet_timer_get_us();
	apic_start_timer(PROCESS_TIME_US);
}



void KERNEL_CALL scheduler_tick(void){
	uint64_t tm=hpet_timer_get_us();
	console_log("Reschedule: %lluus (%u)\n",tm-_l_tm,asm_current_cpu()->id);
	_l_tm=tm;
}
