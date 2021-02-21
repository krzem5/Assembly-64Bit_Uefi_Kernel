ENTRY(_start)
SECTIONS{
    . = 0xffffffffc0000000;
    ___KERNEL_BASE_VA__ = .;
    .text : AT(ADDR(.text) - 0xffffffffc0000000) {
        *(.entry)
        *(.text .text*)
        *(.rodata .rodata* .rdata .rdata*)
    }
    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xffffffffc0000000) {
        *(.data .data*)
    }
    .bss : AT(ADDR(.bss) - 0xffffffffc0000000) {
        *(.bss .bss*)
    }
}
