bits 64
global asm_ide_read
global asm_ide_write



asm_ide_read:
	xor dh, dh
	cmp dl, 8
	jl ._if0
	cmp dl, 12
	jl ._if1
	cmp dl, 14
	jl ._if2
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BMIDE_OFFSET__]
	sub dx, 14
	jmp ._end
._if0:
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BASE_OFFSET__]
	jmp ._end
._if1:
	mov r9w, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_CTRL_OFFSET__]
	add r9w, 2
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BASE_OFFSET__]
	sub dx, 6
	mov al, byte [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_INT__OFFSET__]
	or al, 128
	xchg dx, r9w
	out dx, al
	or al, 127
	xchg dx, r9w
	in al, dx
	xchg dx, r9w
	push rax
	mov al, byte [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_INT__OFFSET__]
	out dx, al
	pop rax
	ret
._if2:
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_CTRL_OFFSET__]
	sub dx, 10
._end:
	in al, dx
	ret



asm_ide_write:
	xor dh, dh
	cmp dl, 8
	jl ._if0
	cmp dl, 12
	jl ._if1
	cmp dl, 14
	jl ._if2
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BMIDE_OFFSET__]
	sub dx, 14
	jmp ._end
._if0:
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BASE_OFFSET__]
	jmp ._end
._if1:
	mov r10w, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_CTRL_OFFSET__]
	add r10w, 2
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_BASE_OFFSET__]
	sub dx, 6
	mov al, byte [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_INT__OFFSET__]
	or al, 128
	xchg dx, r10w
	out dx, al
	or al, 127
	xchg dx, r10w
	mov al, r9b
	out dx, al
	xchg dx, r10w
	mov al, byte [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_INT__OFFSET__]
	out dx, al
	ret
._if2:
	add dx, word [rcx + __C_IDE_DEVICE_CHANNEL_STRUCT_CTRL_OFFSET__]
	sub dx, 10
._end:
	mov al, r9b
	out dx, al
	ret
