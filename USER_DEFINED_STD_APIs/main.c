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
 *               A Single Thread - one Software core Demo
 * ==================================================================== */

int main()
{
	print_vega_startup_design();

    while (1)
    {
        print_string("Choose the operation you want to perform and Enter the corresponding number\r\n\r\n");
        uart_custom_print("%u> Add \r\n%u> Subtract  \r\n%u> Multiply \r\n%u> Divide \r\n%u> Calculate Factorial \r\n%u> Exit\r\n\r\n",
        		ADD, SUBTRACT, MULTIPLY, DIVIDE, FACTORIAL, NO_OP);

        char* input_buf_ptr = uart_scan_uint();

        print_string("\r\n");
        NEXT_XLINE(1);

        /* If the User Input is not a NULL string
         * means there are more than one character in User Input
         * */
        if (input_buf_ptr[0] != '\0')
        {
        	// Convert the input character string to number
            uint32_t num = atoui(input_buf_ptr);

            mathematical_arithmetic_operartors(num);

        }
        NEXT_XLINE(1);

        // Testing the full usage of uart_custom_print API
        uint32_t num_loops = 1;
        uint32_t fact_num = 4;
        int32_t signed_int = -10;

        uint64_t large_factorial = factorial_64(fact_num);
        char status_index = 'A';


        // Fully generic logging engine execution
        uart_custom_print("Checking Custom print API : System Status[%c] = %s | Core Loop Count: %u | Factorial(%u): %lu  |  Signed Number = %i\r\n\n",
                          status_index, "VEGA BOARD SYS UP", num_loops, fact_num, large_factorial, signed_int);
    }

    return 0;
}
