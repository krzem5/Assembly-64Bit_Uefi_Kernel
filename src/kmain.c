#include <kmain.h>
#include <libc/stdint.h>
#include <gfx.h>
#include <paging.h>
#include <driver/console.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/isr.h>
#include <cpu/irq.h>



void __attribute__((ms_abi)) kmain(KernelArgs* ka){
	gfx_init(ka);
	console_init(ka);
	paging_init(ka);
	console_log("Starting System...\n");
	console_log("Memory Map (%llu):\n",ka->mmap_l);
	for (uint64_t i=0;i<ka->mmap_l;i++){
		console_log("  %llx - +%llx",ka->mmap[i].b&0x7fffffffffffffff,ka->mmap[i].l);
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
	// console_log("Setting Up IRQ Handlers...\n");
	// setup_keyboard();
	// setup_timer();
	console_log("Enabling IDT...\n");
	enable_idt();
	console_ok("Reached the End!\n");
}
