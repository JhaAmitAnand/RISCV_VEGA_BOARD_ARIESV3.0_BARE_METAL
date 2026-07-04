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
void print_uint64(uint64_t num)
{
    if (num == 0)
    {
        uart_putc('0');
        return;
    }
    // Buffer for up to max digits of a decimal number possible for uint64_t
    char buf[MAX_NUM_DIGITS_IN_64_BITS+1];

    int index = 0;

    // Extract each digit individually from the number
    while (num > 0)
    {
    	/* Below we don't have buf[index++] = (num % 10) because we want the
    	 * ASCII value to be printed and not the digit directly as we want
    	 * to print the "character" (ASCII) and not the "digit" because
    	 * thats what the processor understands - ASCII, it does not know digit
    	 * ASCII value of digit = digit + ASCII value of 0 */
        buf[index++] = (num % 10) + '0';
        num /= 10;
    }

    /* Print digits ASCII value extracted in reverse order one by one
     * Though we (computer) print ASCII - user will see it as digits */
    while (--index >= 0)
    {
        uart_putc(buf[index]);
    }
}


/* ====================================================================
 * Custom 32-bit decimal printer.
 * Since we don't have a C library, we manually extract digits.
 * ==================================================================== */
void print_uint32(uint32_t num)
{
    if (num == 0)
    {
        uart_putc('0');
        return;
    }
    // Buffer for up to max digits of a decimal number possible for uint32_t
    char buf[MAX_NUM_DIGITS_IN_32_BITS+1];

    int index = 0;

    // Extract each digit individually from the number
    while (num > 0)
    {
    	/* Below we don't have buf[index++] = (num % 10) because we want the
    	 * ASCII value to be printed and not the digit directly as we want
    	 * to print the "character" (ASCII) and not the "digit" because
    	 * thats what the processor understands - ASCII, it does not know digit
    	 * ASCII value of digit = digit + ASCII value of 0 */
        buf[index++] = (num % 10) + '0';
        num /= 10;
    }

    /* Print digits ASCII value extracted in reverse order one by one
     * Though we (computer) print ASCII - user will see it as digits */
    while (--index >= 0)
    {
        uart_putc(buf[index]);
    }
}


/* ====================================================================
 * Custom 16-bit decimal printer.
 * Since we don't have a C library, we manually extract digits.
 * ==================================================================== */
void print_uint16(uint16_t1 num)
{
    if (num == 0)
    {
        uart_putc('0');
        return;
    }
    // Buffer for up to max digits of a decimal number possible for uint32_t
    char buf[MAX_NUM_DIGITS_IN_16_BITS+1];

    int index = 0;

    // Extract each digit individually from the number
    while (num > 0)
    {
    	/* Below we don't have buf[index++] = (num % 10) because we want the
    	 * ASCII value to be printed and not the digit directly as we want
    	 * to print the "character" (ASCII) and not the "digit" because
    	 * thats what the processor understands - ASCII, it does not know digit
    	 * ASCII value of digit = digit + ASCII value of 0 */
        buf[index++] = (num % 10) + '0';
        num /= 10;
    }

    /* Print digits ASCII value extracted in reverse order one by one
     * Though we (computer) print ASCII - user will see it as digits */
    while (--index >= 0)
    {
        uart_putc(buf[index]);
    }
}



void mathematical_arithmetic_operartors(uint32_t num)
{
	switch(num)
	{
	    case ADD:
	    	// #TODO:
	    	print_string("Work in progress. Please choose another operation");
	    	NEXT_XLINE(2);
	    	break;

	    case SUBTRACT:
	    	print_string("Work in progress. Please choose another operation");
	    	NEXT_XLINE(2);
	    	// #TODO:
	        break;

	    case MULTIPLY:
	    	print_string("Work in progress. Please choose another operation");
	    	NEXT_XLINE(2);
	    	// #TODO:
	        break;

	    case DIVIDE:
	    	print_string("Work in progress. Please choose another operation");
	    	NEXT_XLINE(2);
	    	// #TODO:
	        break;

	    case FACTORIAL:
	        print_string("Enter Number below 20 whose factorial is to be found\r\n\r\n");
	        char* input_num_ptr = uart_scan_uint();
	        NEXT_XLINE(2);
	        uint32_t factorial_input_num;
	        if (input_num_ptr[0] != '\0')
	        {
	            // Convert the input character string to number
	            factorial_input_num = atoui(input_num_ptr);
	        }
	        if (factorial_input_num > 20)
	        {
	            print_string("Error: Number greater than 20 overflows even 64-bit registers");
	            NEXT_XLINE(2);
	        }
	        else if(factorial_input_num > 0)
	        {
	            uint64_t result = factorial_64(factorial_input_num);
	            uart_custom_print("Factorial of %u = ",factorial_input_num);
	            print_uint64(result);
	            NEXT_XLINE(1);
	        }
	        else if(factorial_input_num < 0)
	        {
	            print_string("Factorial of Negative Numbers are not defined");
	            NEXT_XLINE(2);
	        }
	        break;

	    case NO_OP:
	    	print_string("Exiting !");
	    	NEXT_XLINE(2);
	        break;
    }
}



