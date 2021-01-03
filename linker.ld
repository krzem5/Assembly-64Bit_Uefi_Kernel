ENTRY(_start)
SECTIONS{
    . = 0xffffffffc0000000;
    __KERNEL_OFFSET__ = 0xffffffffc0000000;
    __KERNEL_START__ = . - 0xffffffffc0000000;
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
    __KERNEL_END__ = . - 0xffffffffc0000000;
    __MAX_KERNEL_HEAP_SIZE__ = 0xffffffffffffffff - 0xffffffffc0000000 - __KERNEL_END__;
}
