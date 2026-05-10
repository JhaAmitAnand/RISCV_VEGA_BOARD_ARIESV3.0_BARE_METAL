/* ====================================================================
 *
 * File Name   : main.c
 * File Type   : Source File (C)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is where the linker comes to pick the 'main'
 * function
 * ==================================================================== */

/* ====================================================================
 *                             INCLUDES
 * ==================================================================== */

#include "hw_drivers.h"
#include "64_bits_maths.h"



/* ====================================================================
 *                         MAIN STARTS HERE
 * ==================================================================== */

int main()
{

	NEXT_XLINE(2);
    print_string("--- VEGA 64-bit Engine is working as Expected ---");
    NEXT_XLINE(2);
    print_string("Note: Max input is 20 for 64-bit results");
    NEXT_XLINE(2);

    while (1)
    {
        print_string("Enter Number: ");

        char* input_buf_ptr = uart_scan_uint();

        print_string("\n");

        /* If the User Input is not a NULL string
         * means there are more than one character in User Input
         * */
        if (input_buf_ptr[0] != '\0')
        {
        	// Convert the input character string to number
            uint32_t num = atoui(input_buf_ptr);

            if (num > 20)
            {
                print_string("Error: Number greater than 20 overflows even 64-bit registers\r\n");
            }
            else
            {
                uint64_t result = factorial_64(num);
                print_string("Result: ");
                print_dec64(result);
                NEXT_XLINE(1);
            }
        }
        NEXT_XLINE(1);
    }

    return 0;
}
