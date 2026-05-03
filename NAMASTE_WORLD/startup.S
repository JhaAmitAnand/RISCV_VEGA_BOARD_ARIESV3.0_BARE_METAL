/* =====================================================
 *
 * File Name   : startup.S
 * File Type   : Assembly File
 * File Author : Amit Anand Jha
 *
 * File Usage  : Technically, we could write a bare-metal program
 * without a dedicated .S file, but in practice, for a RISC-V processor,
 * like the THEJAS32 (ARIES V3.0), it is a functional compulsion
 *
 * With startup.S     : We use the .section .text.entry and the KEEP command
 * in our linker.ld to ensure the CPU hits our assembly code first.
 *
 * Without startup.S  : The linker might place a random C function (like delay)
 * or even a piece of data at 0x00200000. The CPU will try to execute that data
 * as code, leading to an immediate "Illegal Instruction" crash.
 *
 *
 * ===================================================== */


/* Creating a new section. This instructs the Linker to group this code into
 a specific memory "bucket" defined in linker.ld */
.section .text.entry

/* Making a global symbol '_start'. Marks the symbol as "External," allowing the Linker
 to see it across different object files (like the entry point). */
.globl _start


_start:
    /* 1. Setup Stack with 16-byte alignment */
    la sp, _stack_top
    li t0, -16
    and sp, sp, t0

    /* 2. Initialize Global Pointer (No Relaxation) */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    /* 3. Disable Interrupts and Jump */
    csrci mstatus, 0x8
    tail main


/* Making another global symbol '_hang'. Marks the symbol as "External," allowing the Linker
 to see it across different object files (like the entry point).*/
.globl _hang

_hang:
    /* To jump at the address of _hang. address of _hang will be taken from the linker.ld.
    provide a "parking spot." If something goes wrong or the code finishes, i.e. somehow we
    come out of the infinite loop (which is what most embedded systems are), the CPU safely spins
    in a tiny, one-instruction loop forever, doing no harm until you manually reset the board */
    j _hang
