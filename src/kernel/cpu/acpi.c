#include <shared.h>
#include <cpu/acpi.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <kmain.h>
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



struct _ACPI_APIC* _acip_apic;



void KERNEL_CALL acpi_init(KernelArgs* ka){
	console_log("APIC = %p, FADT = %p, HPET = %p\n",ka->apic,ka->fadt,ka->hpet);
	_acip_apic=(struct _ACPI_APIC*)ka->apic;
	uint32_t i=0;
	uint32_t lapic_c=0;
	uint32_t ioapic_c=0;
	uint32_t ov_c=0;
	uint32_t nmi_c=0;
	while (i<_acip_apic->l){
		if (*(_acip_apic->dt+i)==0){
			lapic_c++;
		}
		else if (*(_acip_apic->dt+i)==1){
			ioapic_c++;
		}
		else if (*(_acip_apic->dt+i)==2){
			ov_c++;
		}
		else if (*(_acip_apic->dt+i)==4){
			nmi_c++;
		}
		i+=*(_acip_apic->dt+i+1);
	}
	console_ok("%u, %u, %u, %x\n",lapic_c,ioapic_c,ov_c,nmi_c);
	i=0;
	while (i<_acip_apic->l){
		uint8_t t=*(_acip_apic->dt+i);
		if (t>=0x10){
			i+=*(_acip_apic->dt+i+1);
			continue;
		}
		if (t==0){
			uint8_t p_uid=*(_acip_apic->dt+i+2);
			uint8_t id=*(_acip_apic->dt+i+3);
			uint32_t f=*((uint32_t*)(_acip_apic->dt+i+4));
			if (f){
				if (f&2){
					console_warn("Processor %.2hhx:%.2hhx Not Enabled Yet! (%llx)\n",p_uid,id,f);
				}
				console_log("Found Processor: %.2hhx:%.2hhx\n",p_uid,id);
			}
			i+=8;
		}
		else if (t==1){
			uint32_t addr=*((uint32_t*)(_acip_apic->dt+i+4));
			uint32_t gis_b=*((uint32_t*)(_acip_apic->dt+i+8));
			console_log("I/O APIC: %x:%x\n",addr,gis_b);
			i+=12;
		}
		else if (t==2){
			uint8_t src=*(_acip_apic->dt+i+3);
			uint32_t gis=*((uint32_t*)(_acip_apic->dt+i+4));
			uint16_t f=*((uint16_t*)(_acip_apic->dt+i+8));
			console_log("Override: %u:%x:%hx\n",src,gis,f);
			i+=10;
		}
		else if (t==4){
			uint8_t uid=*(_acip_apic->dt+i+2);
			if (uid!=0xff){
				console_warn("Non-Global NMI!\n");
			}
			uint16_t f=*((uint16_t*)(_acip_apic->dt+i+3));
			uint8_t lint=*(_acip_apic->dt+i+5);
			console_log("NMI: %.2hhx:%hx:%hhu\n",uid,f,lint);
			i+=6;
		}
		else{
			console_warn("Unimplemented Interrupt Controller Structure Type: %#hhx\n",t);
			i+=*(_acip_apic->dt+i+1);
		}
	}
}
