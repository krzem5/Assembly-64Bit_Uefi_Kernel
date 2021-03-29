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



#define CPU_INIT_STACK_SIZE 2048
#define LOW_MEM_AP_INIT_ADDR 0x1000
#define LOW_MEM_AP_PML4_ADDR 0x2000



cpu_t* _cpu_dt;
volatile uint32_t _cpu_ap_c=1;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD cpu_init(void){
	acpi_data_t* dt=acpi_get_data();
	console_log("CPU Count: %u\n",dt->cpu_c);
	_cpu_dt=(cpu_t*)(void*)vm_commit((sizeof(cpu_t)*dt->cpu_c+CPU_INIT_STACK_SIZE*(dt->cpu_c-1)+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2);
	_cpu_dt->s=_cpu_dt;
	_cpu_dt->idx=0;
	asm_cpu_setup_data(_cpu_dt);
	ASSERT(asm_current_cpu()==_cpu_dt);
	console_log("BSP CPU ID: %hhu\nAPIC address: %p, APIC timer t/us: %lu\n",_cpu_dt->id,_cpu_dt->apic,_cpu_dt->apic_tpus);
	vm_identity_map(LOW_MEM_AP_INIT_ADDR,5);
	uint64_t pml4=paging_get_phys_pml4();
	asm_setup_ap_startup_code(pml4);
	cpu_t* ap=_cpu_dt+1;
	uint64_t ap_s=(uint64_t)(void*)_cpu_dt+sizeof(cpu_t)*dt->cpu_c+CPU_INIT_STACK_SIZE;
	uint32_t j=1;
	apic_send_ipi(_cpu_dt->apic,IPI_DESTINATION_SHORTHAND_ALL_WITHOUT_SELF|IPI_LEVEL_ASSERT|IPI_TRIGGER_LEVEL|IPI_DELIVERY_INIT,0,0);
	uint32_t lc=2;
	for (uint32_t i=0;i<dt->cpu_c;i++){
		uint8_t id=dt->cpu[i];
		if (id!=_cpu_dt->id){
			console_log("Enabling CPU#%.2hhx...\n",id);
			ap->s=ap;
			ap->id=id;
			ap->idx=j;
			ap->rsp0=(void*)ap_s;
			asm_setup_ap_startup_data((uint64_t)(void*)ap);
			for (uint8_t k=0;k<200;k++){
				if (k%100){
					apic_send_ipi(_cpu_dt->apic,IPI_LEVEL_ASSERT|IPI_DELIVERY_STARTUP,LOW_MEM_AP_INIT_ADDR>>PAGE_4KB_POWER_OF_2,id);
				}
				if (_cpu_ap_c>=lc){
					goto _init;
				}
				hpet_timer_spinwait(100);
			}
			console_warn("Failed to Initialize CPU#%.2hhx!\n",id);
			_cpu_ap_c++;
			goto _nxt;
_init:
			console_log("CPU#%.2hhx Initialized, Waiting for Setup to Complete...\n",ap->id);
			lc++;
			while (_cpu_ap_c!=lc){
				hpet_timer_spinwait(25);
			}
			console_ok("CPU#%.2hhx Setup Complete!\n",ap->id);
			lc--;
			_cpu_ap_c--;
_nxt:
			lc++;
			ap++;
			ap_s+=CPU_INIT_STACK_SIZE;
			j++;
		}
	}
	console_log("%u/%u CPUs running!\n",_cpu_ap_c,dt->cpu_c);
}
