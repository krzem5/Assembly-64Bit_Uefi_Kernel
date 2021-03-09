bits 64
global asm_mutex_acquire
global asm_mutex_release



asm_mutex_acquire:
	xor rax, rax
	mov dl, 1
	jmp ._start
._loop:
	pause
._start:
	mov byte [rcx], al
	test al, al
	jnz ._loop
	lock cmpxchg byte [rcx], dl
	jnz ._loop
	ret



asm_mutex_release:
	mov byte [rcx], 0
	ret
