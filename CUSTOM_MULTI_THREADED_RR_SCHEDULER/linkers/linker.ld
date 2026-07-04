/* =====================================================
 *
 * File Name   : linker.ld
 * File Type   : Linker File
 * File Author : Amit Anand Jha
 *
 * File Usage  : Technically, we could not write a bare-metal program
 * without a dedicated linker file. This is the first file and the last 
 * file picked. Its basically a link between the startup.S and .O files
 * This defines the mapping of sections and variables and their alignment
 * overall.
 *
 * ===================================================== */

MEMORY
{
    /* Based on THEJAS32 Hardware Specs: SRAM starts at 0x80000000 
    But the bootloader has the IRAM at 0x200000 */
    RAM (rwx) : ORIGIN = 0x200000, LENGTH = 250K
}

SECTIONS
{
    .text :
    {
        _text_start = .;
        KEEP(*(.text.entry)) 
        *(.text*)
        *(.rodata*)    /* Strings live here */
        *(.srodata*)
        . = ALIGN(4);
        _text_end = .;
    } > RAM

    .data : ALIGN(16)
    {
        _data_start = .;
        __global_pointer$ = . + 0x800; /* */
        *(.data*)
        *(.sdata*)
        . = ALIGN(16);
        _data_end = .;
    } > RAM

    .bss : ALIGN(16)
    {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(16);
        _bss_end = .;
    } > RAM

    /* Set stack top to the end of the IRAM range seen in your image
    _stack_top = 0x23E7F0; */
    _stack_top = ALIGN(ORIGIN(RAM) + LENGTH(RAM), 16);
}