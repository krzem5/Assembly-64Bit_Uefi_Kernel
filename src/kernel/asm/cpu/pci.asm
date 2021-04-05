bits 64
global asm_pci_read
global asm_pci_write



asm_pci_read:
	mov eax, ecx
	mov dx, 0xcf8
	out dx, eax
	mov dx, 0xcfc
	in eax, dx
	ret



asm_pci_write:
	mov eax, ecx
	mov ecx, edx
	mov dx, 0xcf8
	out dx, eax
	mov dx, 0xcfc
	mov eax, ecx
	out dx, eax
	ret
