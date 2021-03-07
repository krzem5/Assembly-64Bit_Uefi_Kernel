#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/apic.h>
#include <cpu/cpu.h>
#include <cpu/fatal_error.h>
#include <cpu/msr.h>
#include <gfx/console.h>
#include <memory/vm.h>
#include <stdint.h>



#define CPU_INIT_STACK_SIZE 4096
#define LOW_MEM_AP_INIT_ADDR 0x1000



cpu_t* _cpu_dt;



void KERNEL_CALL cpu_init(void){
	acpi_data_t* dt=acpi_get_data();
	_cpu_dt=(cpu_t*)(void*)vm_commit((sizeof(cpu_t)*dt->cpu_c+CPU_INIT_STACK_SIZE*(dt->cpu_c-1)+4095)>>12);
	_cpu_dt->s=_cpu_dt;
	_cpu_dt->id=apic_get_id();
	_cpu_dt->idx=0;
	_cpu_dt->apic=apic_base();
	apic_enable();
	apic_setup_timer();
	asm_wrmsr(MSR_GS_BASE,(uint64_t)(void*)_cpu_dt);
	ASSERT(current_cpu()==_cpu_dt);
	asm_copy_starup_code(LOW_MEM_AP_INIT_ADDR);
	console_log("CPU Count: %u\n",dt->cpu_c);
	cpu_t* ap=_cpu_dt+1;
	uint64_t ap_s=(uint64_t)(void*)_cpu_dt+sizeof(cpu_t)*dt->cpu_c;
	uint32_t k=1;
	for (uint32_t i=0;i<dt->cpu_c;i++){
		uint8_t id=dt->cpu[i];
		if (id!=_cpu_dt->id){
			console_log("Enabling CPU#%.2hhx\n",id);
			ap_s+=CPU_INIT_STACK_SIZE;
			ap->s=ap;
			ap->id=id;
			ap->idx=k;
			ap->rsp0=(void*)ap_s;
			ap->apic=apic_base();
			ap++;
			k++;
		}
	}
}
