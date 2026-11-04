ENTRY(_start);

SECTIONS
{
    . = 0x40000000;
    .text.boot :  { *(.text.boot) }
    .text :  { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { 
        . = ALIGN(8);
        bss_begin = .;
        *(.bss*) 
        *(COMMON)
        . = ALIGN(8);
        bss_end = .;
    }
    . = ALIGN(16);
    .stack (NOLOAD) : {
        . += 0x4000;
        stack_top = .;
    }
    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
