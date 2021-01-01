bits 64
global asm_halt_cpu



asm_halt_cpu:
	cli
._asm_l:
	hlt
	jmp ._asm_l
