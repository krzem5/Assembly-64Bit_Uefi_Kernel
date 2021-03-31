ENTRY(_start)
SECTIONS{
    ___KERNEL_BASE_VA__ = 0xffffffffc0000000;
    . = ___KERNEL_BASE_VA__;
    .text : AT(ADDR(.text) - ___KERNEL_BASE_VA__) {
        *(.entry)
        *(.text .text*)
        *(.rdata .rdata* .rodata .rodata*)
    }
    .data ALIGN (16) : AT(ADDR(.data) - ___KERNEL_BASE_VA__) {
        *(.data .data* .pdata .pdata* .xdata .xdata*)
    }
    .bss ALIGN (16) : AT(ADDR(.bss) - ___KERNEL_BASE_VA__) {
        *(.bss .bss*)
    }
    .text_unmap ALIGN(16) : AT(ADDR(.text_unmap) - ___KERNEL_BASE_VA__){
        ___KERNEL_UNMAP_START__ = .;
        *(.unmap)
        . = ALIGN(8);
        ___KERNEL_UNMAP_END__ = .;
    }
}
