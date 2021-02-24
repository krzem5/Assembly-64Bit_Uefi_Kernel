ENTRY(_start)
SECTIONS{
    ___KERNEL_BASE_VA__ = 0xffffffffc0000000;
    . = ___KERNEL_BASE_VA__;
    .text : AT(ADDR(.text) - ___KERNEL_BASE_VA__) {
        *(.entry)
        *(.text .text*)
        *(.rodata .rodata* .rdata .rdata*)
    }
    .data ALIGN (0x1000) : AT(ADDR(.data) - ___KERNEL_BASE_VA__) {
        *(.data .data*)
    }
    .bss ALIGN (0x1000) : AT(ADDR(.bss) - ___KERNEL_BASE_VA__) {
        *(.bss .bss*)
    }
}
