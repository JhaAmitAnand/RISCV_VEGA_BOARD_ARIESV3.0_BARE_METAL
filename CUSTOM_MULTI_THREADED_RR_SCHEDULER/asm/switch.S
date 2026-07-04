/* ====================================================================
 * File Name   : switch.S
 * File Type   : Assembly File (RISC-V 32-bit)
 * File Author : Amit Anand Jha

 * File Usage  : Implements the low-level context switch routine for the
 * cooperative multi-thread scheduler on the ARIES VEGA board.
 * ==================================================================== */

.section .text
.globl context_switch
.globl thread_start_stub
.align 2

context_switch:
    # 1. Allocate space and save current thread's context
    addi sp, sp, -64
    sw ra, 0(sp)
    sw s0,  4(sp)
    sw s1,  8(sp)
    sw s2,  12(sp)
    sw s3,  16(sp)
    sw s4,  20(sp)
    sw s5,  24(sp)
    sw s6,  28(sp)
    sw s7,  32(sp)
    sw s8,  36(sp)
    sw s9,  40(sp)
    sw s10, 44(sp)
    sw s11, 48(sp)

    # 2. Save old stack pointer into TCB (*old_sp = sp)
    sw sp, 0(a0)

    # 3. Move new stack pointer address directly into sp (FIXED)
    mv sp, a1

    # 4. Restore new thread's context
    lw ra, 0(sp)
    lw s0,  4(sp)
    lw s1,  8(sp)
    lw s2,  12(sp)
    lw s3,  16(sp)
    lw s4,  20(sp)
    lw s5,  24(sp)
    lw s6,  28(sp)
    lw s7,  32(sp)
    lw s8,  36(sp)
    lw s9,  40(sp)
    lw s10, 44(sp)
    lw s11, 48(sp)

    addi sp, sp, 64
    ret

thread_start_stub:
    mv a0, s0
    mv a1, s1
    jal ra, thread_exit_wrapper
