/* ====================================================================
 *
 * File Name   : main.c
 * File Type   : Source File (C)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is the file where we define some functions to work
 * on 64 bits. Basically all registers on the ARIES VEGA BOARD are 32 bits
 * So to do 64 bit operations - 2 registers need to be stacked together.
 * For that we need to manually do the operations like add, divide, multiply
 * and others : this requires manual touch since the ARIES board only
 * processes 32 bit registers at once and the buses have no clue to transfer
 * 64 bit data : they always talk in terms of 32 bits
 * ==================================================================== */

/* ====================================================================
 *                             INCLUDES
 * ==================================================================== */

#include "hw_drivers.h"
#include "64_bits_maths.h"



/* ====================================================================
 *                         Function Definitions
 * ==================================================================== */


/* ====================================================================
 *                   --- 64-BIT MATH & CONVERSION API ---
 * Calculates factorial using 64-bit arithmetic.
 * On this 32-bit CPU, the compiler will use pairs of registers (e.g., a0, a1)
 * to store and calculate this value.
 * ==================================================================== */
uint64_t factorial_64(uint32_t n) {
    uint64_t res = 1;
    for (uint32_t i = 2; i <= n; i++) {
        res = res*i;
    }
    return res;
}

/* ====================================================================
 * Custom 64-bit decimal printer.
 * Since we don't have a C library, we manually extract digits.
 * ==================================================================== */
void print_dec64(uint64_t num)
{
    if (num == 0)
    {
        uart_putc('0');
        return;
    }
    // Buffer for up to 20 digits (max uint64_t)
    char buf[MAX_NUM_DIGITS_IN_64_BITS+1];
    int index = 0;
    // Extract each digit individually from the number
    while (num > 0)
    {
        buf[index++] = (num % 10) + '0';
        num /= 10;
    }
    /* Print digit extracted in reverse order one by one */
    while (--index >= 0)
    {
        uart_putc(buf[index]);
    }
}
