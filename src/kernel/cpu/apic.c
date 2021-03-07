#include <shared.h>
#include <cpu/apic.h>
#include <cpu/hpet_timer.h>
#include <cpu/isr.h>
#include <gfx/console.h>
#include <stdint.h>



#define APIC_ID_REGISTER 8
#define APIC_VERSION_REGISTER 12
#define APIC_SVR_REGISTER 60
#define APIC_ICR_LOW_REGISER 192
#define APIC_ICR_HIGH_REGISER 196
#define APIC_LVT_TIMER_REGISER 200
#define APIC_TIMER_INIT_REGISER 224
#define APIC_TIMER_VALUE_REGISER 228
#define APIC_TIMER_DIVISOR_REGISER 248
#define APIC_SVR_ENABLE 0x100
#define ISR_SPURIOUS_VECTOR 208
#define APIC_ICR_PENDING 0x1000
#define APIC_TIMER_REPEAT 0x20000
#define APIC_TIMER_CALIB_US 10000
#define UINT32_MAX 0xffffffff



volatile uint32_t* _apic_ptr;



void KERNEL_CALL apic_init(uint64_t b){
	_apic_ptr=(uint32_t*)(void*)b;
}



uint64_t KERNEL_CALL apic_base(void){
	return (uint64_t)(void*)_apic_ptr;
}



void KERNEL_CALL apic_enable(void){
	(*(_apic_ptr+APIC_SVR_REGISTER))|=APIC_SVR_ENABLE;
}



uint8_t KERNEL_CALL apic_get_id(void){
	return (uint8_t)(*(_apic_ptr+APIC_ID_REGISTER)>>24);
}



void KERNEL_CALL apic_setup_timer(void){
	*(_apic_ptr+APIC_LVT_TIMER_REGISER)=ISR_SPURIOUS_VECTOR|APIC_TIMER_REPEAT;
	*(_apic_ptr+APIC_TIMER_DIVISOR_REGISER)=0xb;
	*(_apic_ptr+APIC_TIMER_INIT_REGISER)=UINT32_MAX;
	hpet_timer_spinwait(APIC_TIMER_CALIB_US);
	uint32_t v=*(_apic_ptr+APIC_TIMER_VALUE_REGISER);
	console_log("APIC Timer: %u t/us\n",(UINT32_MAX-v)/APIC_TIMER_CALIB_US);
}




void KERNEL_CALL apic_send_ipi(uint16_t t,uint8_t v,uint8_t d){
	while (*(_apic_ptr+APIC_ICR_LOW_REGISER)&APIC_ICR_PENDING){
		__asm__ volatile("pause");
	}
	*(_apic_ptr+APIC_ICR_HIGH_REGISER)=((uint32_t)d)<<24;
	*(_apic_ptr+APIC_ICR_LOW_REGISER)=(((uint32_t)t)<<8)|v;
}
