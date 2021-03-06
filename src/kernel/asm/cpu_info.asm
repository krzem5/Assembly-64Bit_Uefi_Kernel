bits 64
extern asm_cpuid



asm_cpuid:
	push rbx
	mov rax, rcx
	mov r8, rdx
	cpuid
	mov dword [r8 + __CPUID_INFO_STRUCT_EAX_OFFSET__], eax
	mov dword [r8 + __CPUID_INFO_STRUCT_EBX_OFFSET__], ebx
	mov dword [r8 + __CPUID_INFO_STRUCT_ECX_OFFSET__], ecx
	mov dword [r8 + __CPUID_INFO_STRUCT_EDX_OFFSET__], edx
	pop rbx
	ret
