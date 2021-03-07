bits 16
section .ap_startup
global asm_ap_startup
global asm_ap_startup_len



%define LOAD_ADDR 0x1000
%define TEMP_GDT_CODE 8
%define TEMP_GDT_DATA 16



default rel
asm_ap_startup:
	cli
	cld
	xor ax, ax
	mov ds, ax
	lidt [(LOAD_ADDR + (._idt - asm_ap_startup))]
	mov eax, cr4
	or eax, 10100000b
	mov cr4, eax
	mov eax, [(LOAD_ADDR + (._init_pml4 - asm_ap_startup))]
	mov cr3, eax
	mov eax, 10000000000000000000000000100001b
	mov cr0, eax
	lgdt [(LOAD_ADDR + (._gdt - asm_ap_startup))]
	jmp TEMP_GDT_CODE:(LOAD_ADDR + (._64bit_code - asm_ap_startup))



align 8
._init_cpu_data:
	dq 0
._init_pml4:
	dq 0
._init_func:
	dq 0
align 4
._idt:
	dw 0
	dd 0
align 16
._gdt_dt:
	dq 0
	dq 0x00209a0000000000
	dq 0x0000920000000000
._gdt:
	dw ($ - ._gdt_dt)
	dd (LOAD_ADDR + (._gdt_dt - asm_ap_startup))



bits 64
default abs
._64bit_code:
	mov ax, TEMP_GDT_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov rax, [(LOAD_ADDR + (._init_cpu_data - asm_ap_startup))]
	mov rsp, [rax + __CPU_STRUCT_RSP0_OFFSET__]
	mov rax, [(LOAD_ADDR + (._init_func - asm_ap_startup))]
	jmp rax



asm_ap_startup_len:
	dq ($ - asm_ap_startup)
