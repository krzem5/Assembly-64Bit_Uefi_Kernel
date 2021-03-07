#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/hpet_timer.h>
#include <cpu/irq.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define FEMPTOSECONDS_IN_MICROSECOND 1000000000ull
#define MICROSECONDS_IN_SECOND 1000000ull
#define TIMER_IRQ 0
#define TIMER_ENABLE 4
#define TIMER_PERIODIC 8



volatile uint64_t* _tm_ptr;
uint64_t _tm_p;
uint64_t _tm=0;
uint64_t _tm_us_r=1;
uint64_t _tm_s_r=1;



void _hpet_timer_irq_cb(registers_t* r){
	*(_tm_ptr+33)=1000ull*FEMPTOSECONDS_IN_MICROSECOND/_tm_p;
	scheduler_tick(*(_tm_ptr+30)/_tm_us_r);
}



void KERNEL_CALL hpet_timer_init(uint64_t b){
	console_log("HPET Base Pointer: %p\n",b);
	_tm_ptr=(uint64_t*)(void*)b;
	*(_tm_ptr+2)&=~3;
	*(_tm_ptr+30)=0;
}



void KERNEL_CALL hpet_timer_set_frequency(uint64_t f){
	_tm_p=(*_tm_ptr)>>32;
	_tm_us_r=FEMPTOSECONDS_IN_MICROSECOND/_tm_p;
	_tm_s_r=_tm_us_r*MICROSECONDS_IN_SECOND;
	console_log("HPET Period: %llu\n",_tm_p);
	regiser_irq_handler(TIMER_IRQ,_hpet_timer_irq_cb);
	*(_tm_ptr+30)=0;
	for (uint16_t i=1;i<((*_tm_ptr>>8)&0x1f)+1;i++){
		*(_tm_ptr+32+4*i)=0;
	}
	*(_tm_ptr+33)=f*FEMPTOSECONDS_IN_MICROSECOND/_tm_p;
	*(_tm_ptr+32)=(TIMER_IRQ<<9)|TIMER_PERIODIC|TIMER_ENABLE;
	*(_tm_ptr+2)|=1;
}



uint64_t KERNEL_CALL hpet_timer_get_us(void){
	return *(_tm_ptr+30)/_tm_us_r;
}



uint64_t KERNEL_CALL hpet_timer_get_s(void){
	return *(_tm_ptr+30)/_tm_s_r;
}



void KERNEL_CALL hpet_timer_spinwait(uint64_t us){
	uint64_t e=*(_tm_ptr+30)/_tm_us_r+us;
	while (*(_tm_ptr+30)/_tm_us_r<e){
		__asm__ volatile("pause");
	}
}
