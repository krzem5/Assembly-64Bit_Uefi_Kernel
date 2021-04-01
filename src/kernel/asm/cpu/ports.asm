bits 64
global asm_port_in8
global asm_port_in16
global asm_port_in32
global asm_port_out8
global asm_port_out16
global asm_port_out32



asm_port_in8:
	mov dx, cx
	in al, dx
	ret



asm_port_in16:
	mov dx, cx
	in ax, dx
	ret



asm_port_in32:
	mov dx, cx
	in eax, dx
	ret



asm_port_out8:
	mov al, dl
	mov dx, cx
	out dx, al
	ret



asm_port_out16:
	mov ax, dx
	mov dx, cx
	out dx, ax
	ret



asm_port_out32:
	mov eax, edx
	mov dx, cx
	out dx, eax
	ret
