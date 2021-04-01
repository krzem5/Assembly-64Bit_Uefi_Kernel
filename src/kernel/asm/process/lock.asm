bits 64
global asm_lock_acquire
global asm_lock_release
extern scheduler_yield



asm_lock_acquire:
	xor rax, rax
	mov dl, 1
	jmp ._start
._loop:
	call scheduler_yield
._start:
	mov byte [rcx], al
	test al, al
	jnz ._loop
	lock cmpxchg byte [rcx], dl
	jnz ._loop
	ret



asm_lock_release:
	mov byte [rcx], 0
	ret
