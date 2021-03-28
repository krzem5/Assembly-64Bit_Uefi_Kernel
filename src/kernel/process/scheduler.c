#include <shared.h>
#include <cpu/apic.h>
#include <cpu/cpu.h>
#include <cpu/hpet_timer.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define PROCESS_TIME_US 1000ull



uint8_t scheduler_ready=0;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD scheduler_init(void){

}



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD scheduler_start(void){
	scheduler_ready=1;
	cpu_t* cpu=asm_current_cpu();
	cpu->l_s_tm=hpet_timer_get_us();
}



void KERNEL_CALL scheduler_yield(void){
	cpu_t* cpu=asm_current_cpu();
	uint64_t tm=hpet_timer_get_us();
	if (cpu->idx==0){
		console_log("Reschedule: %lluus (%u)\n",tm-cpu->l_s_tm,cpu->id);
	}
	cpu->l_s_tm=tm;
}
