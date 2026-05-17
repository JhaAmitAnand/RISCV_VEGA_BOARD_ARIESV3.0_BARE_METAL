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
 *                              ENUMS
 * ==================================================================== */
typedef enum
{
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	MODULO,
	FACTORIAL,
	ABSOLUTE,
	NO_OP
}mathematical_op_e;


/* ====================================================================
 *                         Function Declarations
 * ==================================================================== */

void print_uint64(uint64_t n);
void print_uint32(uint32_t n);
void print_uint16(uint16_t1 n);

uint64_t factorial_64(uint32_t n);

void mathematical_arithmetic_operartors(uint32_t num);


#endif /* _64_BITS_MATHS_H */
