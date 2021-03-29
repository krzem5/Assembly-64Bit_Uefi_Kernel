bits 64
global asm_port_in
global asm_port_out



asm_port_in:
	xor rax, rax
	mov dx, cx
	in al, dx
	ret



asm_port_out:
	mov al, dl
	mov dx, cx
	out dx, al
	ret
