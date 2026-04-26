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
    /* Based on THEJAS32 Hardware Specs: SRAM starts at 0x80000000 */
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 256K
}


SECTIONS
{
    .text :
    {
        _text_start = .;
        KEEP(*(.text.entry))
        *(.text*)
        *(.rodata*)
        _text_end = .;
    } > RAM

    /* RISC-V often performs better (and sometimes requires) that data sections
     start on a 4-byte or 8-byte boundary so the alignment is good to have */
    .data : ALIGN(4)
    {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > RAM

    .bss :
    {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM

    /* Stack starts at the very end of the 256KB SRAM. Also since RISC-V calling convention
     requires the stack pointer to be 16-byte aligned at all times, the alignment 
     below is a good to have */
    _stack_top = ALIGN(ORIGIN(RAM) + LENGTH(RAM), 16);
}