bits 64
global _start
extern kmain
extern vm_release
extern ___KERNEL_UNMAP_START__
extern ___KERNEL_UNMAP_END__



section .entry
_start:
	cli
	mov rsp, qword [rcx + __C_KERNEL_ARGS_STRUCT_K_SP_OFFSET__]
	mov rbp, rsp
	call kmain
	mov r8, ___KERNEL_UNMAP_START__
	mov r9, ___KERNEL_UNMAP_END__
	xor rax, rax
	mov rcx, r9
	sub rcx, r8
	shr rcx, 3
	mov rdi, r8
	cld
	rep stosq
	mov rcx, r8
	add rcx, (__C_PAGE_4KB_SIZE__ - 1)
	shr rcx, __C_PAGE_4KB_POWER_OF_2__
	mov rdx, r9
	add rdx, (__C_PAGE_4KB_SIZE__ - 1)
	shr rdx, __C_PAGE_4KB_POWER_OF_2__
	sub rdx, rcx
	shl rcx, __C_PAGE_4KB_POWER_OF_2__
	call vm_release
	sti
._loop:
	hlt
	jmp ._loop
