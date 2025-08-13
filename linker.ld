OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
    . = 0x1000;              /* link/load at 8 KiB */

    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss  : { *(.bss*) *(COMMON) }
}
