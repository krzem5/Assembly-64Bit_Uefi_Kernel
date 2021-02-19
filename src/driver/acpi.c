#include <shared.h>
#include <driver/acpi.h>
#include <libc/stdint.h>
#include <libc/stdlib.h>
#include <kmain.h>
#include <fatal_error.h>
#include <driver/console.h>



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
	uint32_t io_c=0;
	while (i<_acip_apic->l){
		if (*(_acip_apic->dt+i)==1){
			io_c++;
		}
		i+=*(_acip_apic->dt+i+1);
	}
	console_ok("%llx, %llx\n",io_c,_acip_apic->lic_a);
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
		else{
			console_warn("Unimplemented Interrupt Controller Structure Type: %#hhx\n",t);
			i+=*(_acip_apic->dt+i+1);
		}
	}
	asm_halt_cpu();
}
