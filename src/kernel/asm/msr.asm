bits 64
global asm_rdmsr
global asm_wrmsr



asm_rdmsr:
	rdmsr
	shl rdx, 32
	or rax, rdx
	ret



asm_wrmsr:
	xor rax, rax
	mov eax, edx
	shr rdx, 32
	wrmsr
	ret
