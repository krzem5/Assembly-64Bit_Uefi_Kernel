bits 64
global asm_setup_gdt



asm_setup_gdt:
	lgdt [gdt64_pointer]
	mov ax, gdt64_data
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	push byte gdt64_code
	push ._new_cs
	retfq
._new_cs:
	ret



section .data
	gdt64_start:
		dw 0
		dw 0
		db 0
		db 0
		db 0
		db 0
	gdt64_code: equ ($ - gdt64_start)
		dw 0
		dw 0
		db 0
		db 10011010b
		db 10100000b
		db 0
	gdt64_data: equ ($ - gdt64_start)
		dw 0
		dw 0
		db 0
		db 10010010b
		db 10000000b
		db 0
	gdt64_pointer:
		dw $-gdt64_start-1
		dq gdt64_start
