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
    /* To Disable interrupts : Directly modifies the Control and Status Register (CSR)
    file logic to CLEAR IMMEDIATE (csr'CR') bits. Without this, an unexpected interrupt would
    force the CPU to jump to a "Trap Vector" that doesn't exist, causing a permanent hang.
    We specifically set the 3rd bit, the MIE bit (Machine Interrupt Enable bit)*/
    csrci mstatus, 0x8

    /* Setup Stack Pointer for the 0x00200000 RAM range
    Load Address for Stack Pointer (sp) as defined in linker.ld */
    la sp, _stack_top

    /* Clear return address and jump to main
    Load Immediate value of 0 in the RA (Return Address) register
    Loading 0 into RA, we explicitly tell any connected debugger or the CPU itself
    that there is nowhere to return to. This defines the absolute start of the
    program's execution history. If ra is 0, the debugger understands it has reached the
    "root" or the "entry point" of the software and stops searching. */
    li ra, 0

    /* The tail instruction is a pseudo-instruction in RISC-V assembly that
    performs a tail-call optimization. It is effectively an unconditional jump to
    the address of the main function. Address of main function comes from linker.ld
    We explicitly set the RA register to 0. Using tail ensures that RA remains 0 when
    main starts, which tells debuggers that main is the very first function in the execution chain*/
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
