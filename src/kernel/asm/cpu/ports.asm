bits 64
global asm_port_in
global asm_port_in16
global asm_port_in32
global asm_port_out
global asm_port_out16
global asm_port_out32



asm_port_in:
	xor rax, rax
	mov dx, cx
	in al, dx
	ret



asm_port_in16:
	xor rax, rax
	mov dx, cx
	in ax, dx
	ret



asm_port_in32:
	xor rax, rax
	mov dx, cx
	in eax, dx
	ret



asm_port_out:
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
