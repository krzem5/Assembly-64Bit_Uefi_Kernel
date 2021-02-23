#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/irq.h>
#include <cpu/timer.h>
#include <gfx/console.h>
#include <process/scheduler.h>



#define SECONDS_TO_FEMTOSECONDS(n) ((n)*1000000000000000ull)
#define TIMER_FREQUENCY 1000000llu



uint64_t* _tm_ptr;
uint64_t _tm_freq=1;
uint64_t _tm_upd;
uint64_t _tm=0;



void _timer_irq_cb(registers_t* r){
	*(_tm_ptr+4)|=1;
	*(_tm_ptr+33)=*(_tm_ptr+30)+_tm_upd;
	_tm=*(_tm_ptr+30);
	scheduler_tick(_tm/(_tm_freq/1000000llu));
}



void KERNEL_CALL timer_init(uint64_t b){
	console_log("HPET Base Ptr: %p\n",b);
	_tm_ptr=(uint64_t*)(void*)b;
	*(_tm_ptr+2)&=~1;
	uint64_t of=SECONDS_TO_FEMTOSECONDS(1)/((*_tm_ptr)>>32);
	_tm_freq=(of>TIMER_FREQUENCY?TIMER_FREQUENCY:of);
	_tm_upd=of/_tm_freq;
	console_log("HPET Data: period = %llu, frequency = %lluHz\n",(*_tm_ptr)>>32,of);
	regiser_irq_handler(0,_timer_irq_cb);
	*(_tm_ptr+30)=0;
	*(_tm_ptr+32)&=~8;
	*(_tm_ptr+32)|=4;
	*(_tm_ptr+33)=_tm_upd;
	*(_tm_ptr+2)|=3;
}



uint64_t KERNEL_CALL timer_get_us(void){
	return _tm/(_tm_freq/1000000llu);
}



uint64_t KERNEL_CALL timer_get_s(void){
	return _tm/_tm_freq;
}
