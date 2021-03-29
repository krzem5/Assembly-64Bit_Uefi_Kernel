bits 16
default rel
global asm_ap_startup
extern cpu_ap_init
extern asm_setup_gdt
extern asm_apic_setup
extern scheduler_start
extern idt
extern scheduler_ready
extern _cpu_ap_c



section .unmap
asm_ap_startup:
	cli
	cld
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	lidt [(__C_LOW_MEM_AP_INIT_ADDR__ + (._idt - asm_ap_startup))]
	mov eax, cr4
	bts eax, 5
	mov cr4, eax
	mov eax, __C_LOW_MEM_AP_PML4_ADDR__
	mov cr3, eax
	mov ecx, 0xc0000080
	rdmsr
	bts eax, 8
	wrmsr
	mov eax, cr0
	or eax, 10000000000000000000000000000001b
	mov cr0, eax
	lgdt [(__C_LOW_MEM_AP_INIT_ADDR__ + (._gdt_ptr - asm_ap_startup))]
	jmp ._gdt_code:(__C_LOW_MEM_AP_INIT_ADDR__ + (._64bit_code - asm_ap_startup))



align 8
._init_pml4:
	dq 0
._init_cpu_data:
	dq 0
align 4
._idt:
	dw 0
	dd 0
align 16
._gdt:
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0
._gdt_code: equ ($ - ._gdt)
	dw 0
	dw 0
	db 0
	db 10011010b
	db 10100000b
	db 0
._gdt_data: equ ($ - ._gdt)
	dw 0
	dw 0
	db 0
	db 10010010b
	db 10000000b
	db 0
._gdt_ptr:
	dw $-._gdt-1
	dq (__C_LOW_MEM_AP_INIT_ADDR__ + (._gdt - asm_ap_startup))



bits 64
align 8
default abs
._64bit_code:
	mov ax, ._gdt_data
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov rax, qword [(__C_LOW_MEM_AP_INIT_ADDR__ + (._init_pml4 - asm_ap_startup))]
	mov cr3, rax
	mov r8, qword [(__C_LOW_MEM_AP_INIT_ADDR__ + (._init_cpu_data - asm_ap_startup))]
	mov rbp, qword [r8 + __C_CPU_STRUCT_RSP0_OFFSET__]
	mov rsp, rbp
	mov rax, _asm_ap_startup_kernel
	jmp rax



align 8
asm_ap_startup_len equ ($ - asm_ap_startup)
%if asm_ap_startup_len > __C_PAGE_4KB_SIZE__
%error AP Startup Code too long!
%endif



global asm_setup_ap_startup_code
global asm_setup_ap_startup_data



_asm_ap_startup_kernel:
	inc dword [_cpu_ap_c]
	mov rcx, __C_MSR_APIC_BASE__
	rdmsr
	shl rdx, 32
	or rax, rdx
	xor ax, ax
	mov qword [r8 + __C_CPU_STRUCT_APIC_OFFSET__], rax
	push rax
	xor rax, rax
	mov eax, r8d
	mov rcx, __C_MSR_GS_BASE__
	mov rdx, r8
	shr rdx, 32
	wrmsr
	pop rcx
	call asm_apic_setup
	lidt [idt]
	call asm_setup_gdt
	jmp _asm_ap_startup_kernel_wait



section .text
_asm_ap_startup_kernel_wait:
	inc dword [_cpu_ap_c]
	jmp ._start_w
._w_loop:
	pause
._start_w:
	cmp byte [scheduler_ready], 1
	jne ._w_loop
	call scheduler_start
	sti
._loop:
	hlt
	jmp ._loop



section .unmap
asm_setup_ap_startup_code:
	push rdi
	push rsi
	mov rdx, rcx
	mov rdi, __C_LOW_MEM_AP_INIT_ADDR__
	mov rsi, asm_ap_startup
	mov rcx, ((asm_ap_startup_len + __C_SIZEOF_UINT64_T__ - 1) / __C_SIZEOF_UINT64_T__)
	rep movsq
	mov rdi, __C_LOW_MEM_AP_PML4_ADDR__
	xor rax, rax
	mov rcx, ((__C_PAGE_4KB_SIZE__ * 4) / __C_SIZEOF_UINT64_T__)
	rep stosq
	mov qword [(__C_LOW_MEM_AP_INIT_ADDR__ + (asm_ap_startup._init_pml4 - asm_ap_startup))], rdx
	mov rdi, __C_LOW_MEM_AP_PML4_ADDR__
	mov qword [rdi], ((__C_LOW_MEM_AP_PML4_ADDR__ + __C_PAGE_4KB_SIZE__) | __C_PAGE_DIR_READ_WRITE__ | __C_PAGE_DIR_PRESENT__)
	add rdi, __C_PAGE_4KB_SIZE__
	mov qword [rdi], ((__C_LOW_MEM_AP_PML4_ADDR__ + __C_PAGE_4KB_SIZE__ * 2) | __C_PAGE_DIR_READ_WRITE__ | __C_PAGE_DIR_PRESENT__)
	add rdi, __C_PAGE_4KB_SIZE__
	mov qword [rdi], ((__C_LOW_MEM_AP_PML4_ADDR__ + __C_PAGE_4KB_SIZE__ * 3) | __C_PAGE_DIR_READ_WRITE__ | __C_PAGE_DIR_PRESENT__)
	add rdi, (__C_PAGE_4KB_SIZE__ + (__C_LOW_MEM_AP_INIT_ADDR__ / __C_PAGE_4KB_SIZE__) * __C_SIZEOF_UINT64_T__)
	mov qword [rdi], (__C_LOW_MEM_AP_INIT_ADDR__ | __C_PAGE_READ_WRITE__ | __C_PAGE_PRESENT__)
	pop rsi
	pop rdi
	ret



asm_setup_ap_startup_data:
	mov qword [(__C_LOW_MEM_AP_INIT_ADDR__ + (asm_ap_startup._init_cpu_data - asm_ap_startup))], rcx
	ret
