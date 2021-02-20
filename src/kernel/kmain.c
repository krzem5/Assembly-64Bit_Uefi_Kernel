#include <shared.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/irq.h>
#include <cpu/isr.h>
#include <driver/acpi.h>
#include <driver/console.h>
#include <gfx.h>
#include <kmain.h>
#include <memory/paging.h>
#include <stdint.h>
#include <stdlib.h>



void KERNEL_CALL kmain(KernelArgs* ka){
	paging_init(ka);
	gfx_init(ka);
	console_init(ka);
	console_log("Starting System...\n");
	console_log("Memory Map (%llu):\n",ka->mmap_l);
	for (uint64_t i=0;i<ka->mmap_l;i++){
		console_log("  %llx - +%llx",ka->mmap[i].b&0xfffffffffffffffe,ka->mmap[i].l);
		if (ka->mmap[i].b>>63){
			console_log(" (ACPI Tables)");
		}
		console_log("\n");
	}
	console_log("Setting Up GDT...\n");
	asm_setup_gdt();
	console_log("Setting Up IDT...\n");
	setup_idt(ka);
	console_log("Setting Up Default ISRs...\n");
	setup_isr();
	console_log("Setting Up Default IRQs...\n");
	setup_irq();
	console_log("Enabling IDT...\n");
	enable_idt();
	void* ptr=malloc(100);
	console_log("abc %p\n",ptr);
	free(ptr);
	// console_log("Setting Up ACPI...\n");
	// acpi_init(ka);
	console_ok("Reached the End!\n");
	asm_end_loop();
}
