#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/ap_startup.h>
#include <cpu/apic.h>
#include <cpu/cpu.h>
#include <cpu/fatal_error.h>
#include <cpu/hpet_timer.h>
#include <cpu/msr.h>
#include <gfx/console.h>
#include <memory/paging.h>
#include <memory/vm.h>
#include <stdint.h>



#define CPU_INIT_STACK_SIZE 1024
#define LOW_MEM_AP_INIT_ADDR 0x1000
#define LOW_MEM_AP_PML4_ADDR 0x2000



cpu_t* _cpu_dt;
volatile uint32_t _cpu_ap_c=1;



void KERNEL_CALL cpu_init(void){
	acpi_data_t* dt=acpi_get_data();
	console_log("CPU Count: %u\n",dt->cpu_c);
	_cpu_dt=(cpu_t*)(void*)vm_commit((sizeof(cpu_t)*dt->cpu_c+CPU_INIT_STACK_SIZE*(dt->cpu_c-1)+4095)>>12);
	_cpu_dt->s=_cpu_dt;
	_cpu_dt->id=apic_get_id();
	_cpu_dt->idx=0;
	_cpu_dt->apic=apic_base();
	apic_enable();
	apic_setup_timer();
	asm_wrmsr(MSR_GS_BASE,(uint64_t)(void*)_cpu_dt);
	ASSERT(current_cpu()==_cpu_dt);
	vm_identity_map(LOW_MEM_AP_INIT_ADDR,5);
	uint64_t pml4=paging_get_phys_pml4();
	asm_init_ap_startup_code(pml4);
	cpu_t* ap=_cpu_dt+1;
	uint64_t ap_s=(uint64_t)(void*)_cpu_dt+sizeof(cpu_t)*dt->cpu_c;
	uint32_t j=1;
	apic_send_ipi(IPI_DESTINATION_SHORTHAND_ALL_WITHOUT_SELF|IPI_LEVEL_ASSERT|IPI_TRIGGER_LEVEL|IPI_DELIVERY_INIT,0,0);
	uint32_t lc=2;
	for (uint32_t i=0;i<dt->cpu_c;i++){
		uint8_t id=dt->cpu[i];
		if (id!=_cpu_dt->id){
			console_log("Enabling CPU#%.2hhx\n",id);
			ap->s=ap;
			ap->id=id;
			ap->idx=j;
			ap->rsp0=(void*)ap_s;
			ap->apic=apic_base();
			asm_setup_ap_startup_vars((uint64_t)(void*)ap);
			apic_send_ipi(IPI_LEVEL_ASSERT|IPI_DELIVERY_STARTUP,LOW_MEM_AP_INIT_ADDR>>12,id);
			for (uint8_t k=0;k<100;k++){
				if (_cpu_ap_c==lc){
					goto _nxt;
				}
				hpet_timer_spinwait(100);
			}
			apic_send_ipi(IPI_LEVEL_ASSERT|IPI_DELIVERY_STARTUP,LOW_MEM_AP_INIT_ADDR>>12,id);
			for (uint8_t k=0;k<100;k++){
				if (_cpu_ap_c==lc){
					goto _nxt;
				}
				hpet_timer_spinwait(100);
			}
			console_warn("Failed to Init CPU#%.2hhx!\n",id);
_nxt:
			lc++;
			ap++;
			ap_s+=CPU_INIT_STACK_SIZE;
			j++;
		}
	}
}



void KERNEL_CALL cpu_ap_init(cpu_t* cpu){
	_cpu_ap_c++;
}
