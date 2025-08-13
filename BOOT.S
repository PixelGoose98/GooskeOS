BITS 32
section .multiboot_header
align 4
    dd 0x1BADB002           ; magic number for GRUB
    dd 0x00000000           ; flags
    dd -(0x1BADB002 + 0x00000000) ; checksum

section .text
global start
extern kmain

start:
    cli
    mov esp, stack_top
    call kmain
    hlt

section .bss
align 16
stack_bottom:
    resb 8192
stack_top:
