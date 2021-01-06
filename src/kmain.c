#include <kmain.h>
#include <libc/stdint.h>
#include <gfx.h>
#include <heap.h>
#include <driver/console.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/isr.h>
#include <cpu/irq.h>



void __attribute__((ms_abi)) kmain(KernelArgs* ka){
	gfx_init(ka);
	console_init(ka);
	init_heap(ka);
	console_log("Starting System...\n");
	console_log("Memory Map (%llu):\n",ka->mmap_l);
	for (uint64_t i=0;i<ka->mmap_l;i++){
		console_log("  %llx - +%llx (%u)\n",ka->mmap[i].b&0x7fffffffffffffff,ka->mmap[i].l,ka->mmap[i].b>>63);
	}
	console_log("Setting Up GDT...\n");
	asm_setup_gdt();
	console_log("Setting Up IDT...\n");
	setup_idt();
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
