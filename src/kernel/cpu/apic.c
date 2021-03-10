#include <shared.h>
#include <cpu/apic.h>
#include <cpu/fatal_error.h>
#include <cpu/hpet_timer.h>
#include <cpu/idt.h>
#include <gfx/console.h>
#include <process/scheduler.h>
#include <stdint.h>



#define APIC_ID_REGISTER 8
#define APIC_EOI_REGISTER 44
#define APIC_SPURIOUS_REGISTER 60
#define APIC_ICR_LOW_REGISER 192
#define APIC_ICR_HIGH_REGISER 196
#define APIC_LVT_TIMER_REGISER 200
#define APIC_LVT_LINT0_REGISER 212
#define APIC_LVT_LINT1_REGISER 216
#define APIC_LVT_ERROR_REGISER 220
#define APIC_TIMER_INIT_REGISER 224
#define APIC_TIMER_VALUE_REGISER 228
#define APIC_TIMER_DIVISOR_REGISER 248
#define APIC_SVR_ENABLE 0x100
#define APIC_ICR_PENDING 0x1000
#define APIC_TIMER_REPEAT 0x20000
#define APIC_TIMER_CALIB_US 10000



volatile uint32_t* _apic_ptr;
uint64_t _apic_t_per_us;



void KERNEL_CALL _handle_apic_int(apic_interrupt_registers_t* r){
	if (r->t==APIC_TIMER_INTERRUPT){
		scheduler_yield();
	}
	else{
		console_warn("APIC:\n  rax    = %#.18llx\n  rbx    = %#.18llx\n  rcx    = %#.18llx\n  rdx    = %#.18llx\n  rsi    = %#.18llx\n  rdi    = %#.18llx\n  rbp    = %#.18llx\n  r8     = %#.18llx\n  r9     = %#.18llx\n  r10    = %#.18llx\n  r11    = %#.18llx\n  r12    = %#.18llx\n  r13    = %#.18llx\n  r14    = %#.18llx\n  r15    = %#.18llx\n  rflags = %#.8llx\n  t      = %#.4llx\n",r->rax,r->rbx,r->rcx,r->rdx,r->rsi,r->rdi,r->rbp,r->r8,r->r9,r->r10,r->r11,r->r12,r->r13,r->r14,r->r15,r->rflags,r->t);
	}
}



void KERNEL_CALL apic_init(void){
	set_idt_entry(APIC_SPURIOUS_INTERRUPT,asm_apic_spurious_int,0x08,0x8e);
	set_idt_entry(APIC_TIMER_INTERRUPT,asm_apic_timer_int,0x08,0x8e);
	set_idt_entry(APIC_LINT0_INTERRUPT,asm_apic_lint0_int,0x08,0x8e);
	set_idt_entry(APIC_LINT1_INTERRUPT,asm_apic_lint1_int,0x08,0x8e);
	set_idt_entry(APIC_ERROR_INTERRUPT,asm_apic_err_int,0x08,0x8e);
}



void KERNEL_CALL apic_send_ipi(uint64_t apic,uint16_t t,uint8_t v,uint8_t d){
	volatile uint32_t* ptr=(uint32_t*)(void*)apic;
	while (*(ptr+APIC_ICR_LOW_REGISER)&APIC_ICR_PENDING){
		__asm__ volatile("pause");
	}
	*(ptr+APIC_ICR_HIGH_REGISER)=((uint32_t)d)<<24;
	*(ptr+APIC_ICR_LOW_REGISER)=(((uint32_t)t)<<8)|v;
}
