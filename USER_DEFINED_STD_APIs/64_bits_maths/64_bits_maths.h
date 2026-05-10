/* ====================================================================
 *
 * File Name   : 6_bits_naths.h
 * File Type   : Header File (C Header File)
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

#ifndef _64_BITS_MATHS_H
#define _64_BITS_MATHS_H


/* ====================================================================
 *                             INCLUDES
 * ==================================================================== */
#include "hw_drivers.h"


/* ====================================================================
 *                         Function Declarations
 * ==================================================================== */

void print_dec64(uint64_t n);
uint64_t factorial_64(uint32_t n);


#endif /* _64_BITS_MATHS_H */
