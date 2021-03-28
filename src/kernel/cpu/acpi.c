#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/apic.h>
#include <cpu/fatal_error.h>
#include <cpu/hpet_timer.h>
#include <gfx/console.h>
#include <kmain.h>
#include <memory/vm.h>
#include <stdint.h>
#include <stdlib.h>



struct _ACPI_APIC{
	uint32_t t;
	uint32_t l;
	uint8_t rv;
	uint8_t c;
	char oem_id[6];
	char oem_t[8];
	uint32_t oem_rv;
	uint32_t c_id;
	uint32_t c_rv;
	uint32_t lic_a;
	uint32_t f;
	uint8_t dt[];
} __attribute__ ((packed));



struct _ACPI_HPET_ADDR{
	uint8_t t;
	uint8_t r_b;
	uint8_t r_off;
	uint8_t a_sz;
	uint64_t a;
} __attribute__ ((packed));



struct _ACPI_HPET{
	uint32_t t;
	uint32_t l;
	uint8_t rv;
	uint8_t c;
	char oem_id[6];
	char oem_t[8];
	uint32_t oem_rv;
	uint32_t c_id;
	uint32_t c_rv;
	uint32_t h_id;
	struct _ACPI_HPET_ADDR b_addr;
	uint8_t i;
	uint16_t pm;
	uint8_t a;
} __attribute__ ((packed));



acpi_data_t* _dt_ptr;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD acpi_init(KernelArgs* ka){
	console_log("ACPI Pointers: APIC = %p, FADT = %p, HPET = %p\n",ka->apic,ka->fadt,ka->hpet);
	struct _ACPI_APIC* apic=(struct _ACPI_APIC*)ka->apic;
	uint32_t i=0;
	uint32_t lapic_c=0;
	uint32_t ioapic_c=0;
	uint32_t ov_c=0;
	uint32_t nmi_c=0;
	while (i<apic->l){
		if (*(apic->dt+i)==0){
			lapic_c++;
		}
		else if (*(apic->dt+i)==1){
			ioapic_c++;
		}
		else if (*(apic->dt+i)==2){
			ov_c++;
		}
		else if (*(apic->dt+i)==4){
			nmi_c++;
		}
		i+=*(apic->dt+i+1);
	}
	console_log("lAPIC = %u, ioAPIC = %u, Override = %u, NMI = %x\n",lapic_c,ioapic_c,ov_c,nmi_c);
	_dt_ptr=(acpi_data_t*)(void*)vm_commit((sizeof(acpi_data_t)+sizeof(uint8_t)*lapic_c+4095)>>12);
	_dt_ptr->cpu_c=lapic_c;
	i=0;
	uint32_t j=0;
	while (i<apic->l){
		uint8_t t=*(apic->dt+i);
		if (t>=0x10){
			i+=*(apic->dt+i+1);
			continue;
		}
		if (t==0){
			uint8_t p_uid=*(apic->dt+i+2);
			uint8_t id=*(apic->dt+i+3);
			uint32_t f=*((uint32_t*)(apic->dt+i+4));
			if (f){
				if (f&2){
					console_warn("Processor %.2hhx:%.2hhx Not Enabled Yet! (%llx)\n",p_uid,id,f);
				}
				else{
					_dt_ptr->cpu[j]=id;
					j++;
				}
				console_log("Found Processor: %.2hhx:%.2hhx\n",p_uid,id);
			}
			i+=8;
		}
		else if (t==1){
			uint32_t addr=*((uint32_t*)(apic->dt+i+4));
			uint32_t gis_b=*((uint32_t*)(apic->dt+i+8));
			console_log("I/O APIC: %x:%x\n",addr,gis_b);
			i+=12;
		}
		else if (t==2){
			uint8_t src=*(apic->dt+i+3);
			uint32_t gis=*((uint32_t*)(apic->dt+i+4));
			uint16_t f=*((uint16_t*)(apic->dt+i+8));
			console_log("Override: %u:%x:%hx\n",src,gis,f);
			i+=10;
		}
		else if (t==4){
			uint8_t uid=*(apic->dt+i+2);
			if (uid!=0xff){
				console_warn("Non-Global NMI!\n");
			}
			uint16_t f=*((uint16_t*)(apic->dt+i+3));
			uint8_t lint=*(apic->dt+i+5);
			console_log("NMI: %.2hhx:%hx:%hhu\n",uid,f,lint);
			i+=6;
		}
		else{
			console_warn("Unimplemented Interrupt Controller Structure Type: %#hhx\n",t);
			i+=*(apic->dt+i+1);
		}
	}
	hpet_timer_init(((struct _ACPI_HPET*)ka->hpet)->b_addr.a);
}



acpi_data_t* KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD acpi_get_data(void){
	return _dt_ptr;
}



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD acpi_free_data(void){
	console_warn("Not Implemented!\n");
}
