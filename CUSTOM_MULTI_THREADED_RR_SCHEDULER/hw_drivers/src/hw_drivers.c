/* ====================================================================
 *
 * File Name   : hw_drivers.c
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



/* ====================================================================
 *                     UART DRIVER FUNCTION DEFINITIONS
 * ==================================================================== */


/* ====================================================================
 *  A simple function that prints the design at the start for reference
 * ==================================================================== */
void print_vega_startup_design()
{
	NEXT_XLINE(2);
	print_string("\r\n+--------------------------------------------------+");
    print_string("\r\n|           VEGA 32-bit ARIES V.3 BOARD            |");
    print_string("\r\n|                                                  |");
    print_string("\r\n|             MADE IN BHARAT : BY CDAC             |");
    print_string("\r\n+--------------------------------------------------+");
    print_string("\r\n|     VEGA 32-bit Engine |  RISC V ARCHITECTURE    |");
    print_string("\r\n|                                                  |");
    print_string("\r\n|       VEGA ET1031 | THEJAS32 | UART XMODEM       |");
    print_string("\r\n+--------------------------------------------------+");
    print_string("\r\n|   Custom Functions written by : Amit Anand Jha   |");
    print_string("\r\n|      RAM : ORIGIN = 0x200000, LENGTH = 250K      |");
    print_string("\r\n|    Reach out at : amitanandjha12@zohomail.in     |");
	print_string("\r\n+--------------------------------------------------+");
    NEXT_XLINE(2);
}



/* ====================================================================
 * Driver for ARIESV3.0 to print character given as input to this API
 * Before printing we check if the board is ready to pick the new char
 * to be printed or not - thats the while check. Check MACRO definitions
 * ==================================================================== */
void uart_putc(char c)
{
    /* Wait for Transmit Holding Register to be empty */
    while (!(UART_LSR & UART_LSR_THRE));
    UART_THR = c;
}



/* ====================================================================
 * Driver for ARIESV3.0 to print any string given as input to this API
 * This API uses the uart_putc API internally. The entire string is basically
 * parsed to each individual character and then printed unless we encounter
 * a Null Terminator for strings - \0 is what strings end with
 * ==================================================================== */
void print_string(const char* s)
{
    while (*s != '\0') {
        uart_putc(*s++);   // same as uart_putc(*s); s=s+1;
    }
}


/* ====================================================================
 * Driver for ARIESV3.0 to take any string given as input to the HW board
 * This API checks if the 0th bit of the Line Status Register is 1 or 0
 * and accepts the user input if the line is not already busy
 * ==================================================================== */
char uart_getc()
{
    /* Wait until a character is received */
    while (!(UART_LSR & UART_LSR_DR));
    return (char)UART_RBR;
}



/* ====================================================================
 * Simple function to accept multi-digit user input as a string
 * This function is not compatible to accept negative inputs as
 * the parsing logic of the function does not account for it (- character)
 * ==================================================================== */
char* uart_scan_uint(void)
{
	// Allocate the max buffer size we can supposedly ever use
    static char input_buf[MAX_NUM_CHARS_IN_USER_INPUT];
    int idx = 0;

    /* Collect multi-digit input from User */
    while (1)
    {
	    char usr_input = uart_getc();

	    // If User pressed "Enter/Return"
        if (usr_input == '\r' || usr_input == '\n')
        {
            // If User entered "Enter" after entering few digits
        	if(idx > 0)
        	{
                // End the string and move cursor to new line on terminal and exit the loop
                input_buf[idx] = '\0';
                NEXT_XLINE(1);
                break;
        	}
        	// If User entered "Enter" without typing digits ignore & print prompt
        	else
        	{
        		print_string("Pressed Enter without entering any number. Enter the number\r\n");
        		continue;
        	}
        }
        // If User pressed "Backspace"
        else if (usr_input == 8 || usr_input == 127)
        {
    	    /* Go back 1 place in the buffer (Recall \b is the Backspace Control Character)
    	     * and replace the last character on terminal with a " "(space)
    	     * then move the cursor one more space back for new entry to be typed
    	     * */
            if (idx > 0)
            {
                idx--;
                print_string("\b \b");
             }
         }
         /* If user enters any digit between 0 to 9 and there is still buffer space left
          * to accept the input then save input in buffer and display the digit on terminal
          * Always leave the last index for the Null Terminator of strings '\0'  */
         else if (usr_input >= '0' && usr_input <= '9' && idx < (MAX_NUM_CHARS_IN_USER_INPUT-1))
         {
             input_buf[idx++] = usr_input;
             uart_putc(usr_input);
         }
    }
    return input_buf;
}


/* ====================================================================
 *  String (ASCII) to unsigned integer converter for multi-digit input
 * ==================================================================== */
uint32_t atoui(const char* s)
{
    uint32_t res = 0;
    while (*s >= '0' && *s <= '9')
    {
    	/* Below we don't have res = res*10 + *s because we want the
    	 * digits and not its ASCII value to be added so to get the
    	 * digit to be added, we need to implement the logic -
    	 * digit = ASCII value of digit - ASCII value of 0 */
        res = res * 10 + (*s - '0');
        s++;
    }
    return res;
}


/* ====================================================================
 *   Defining the delay API which can basically help to insert delays
 *   It uses the ASSEMBLY defined 'NOP' operation underneath to create
 *   cycle delays, where the Processor basically sits IDLE
 * ==================================================================== */
void delay(volatile int count)
{
    while(count--) {
    	DO_NOTHING_CONSUME_A_CYCLE;
    }
}



/* ====================================================================
 *  Function that can be used specifically to print signed numbers only
 * ==================================================================== */
static void uart_print_signed32(int32_t num)
{
    if (num < 0)
    {
        uart_putc('-');

        /* Explicit edge-case guard for -2147483648 to prevent infinite
         * inversion loops due to 2's complement hardware boundaries */
        if (num == -2147483648)
        {
            print_string("2147483648");
            return;
        }
        num = -num;
    }
    print_uint32((uint32_t)num);
}



void uart_custom_print(const char *format, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, format); // Bind to your argument stream

    // Print until we reach the string end
    while (*format != '\0')
    {
    	// Directly print the character if it is not a format specifier
        if (*format != '%')
        {
            uart_putc(*format);
            format++; // Advance smoothly to the next string character
        }

        // If a format specifier is encountered print corresponding argument that was passed
        else if (*format == '%')
        {
            format++; // Move past '%' to find the specifier character

            switch (*format)
            {
                case PRINT_CHARACTER:
                {
                    // Variadic promotion treats chars as standard ints
                    char c = (char)__builtin_va_arg(args, int);
                    uart_putc(c);
                    break;
                }
                case PRINT_STRING:
                {
                    char *s = __builtin_va_arg(args, char*);
                    if (s == NULL) {
                        print_string("(null)");
                    } else {
                        print_string(s);
                    }
                    break;
                }
                case PRINT_POSITIVE_DECIMAL_NUMBER:
                {
                    uint32_t u = __builtin_va_arg(args, uint32_t);
                    print_uint32(u);
                    break;
                }
                case PRINT_SIGNED_DECIMAL_NUMBER:
                {
                    int32_t d = __builtin_va_arg(args, int32_t);
                    uart_print_signed32(d);
                    break;
                }
                case 'l':
                {
                	if(*(format + 1) == 'u')
                	{
                		format = format + 1;
                        // Leverages your custom register-stacking 64-bit math module!
                        uint64_t lu = __builtin_va_arg(args, uint64_t);
                        print_uint64(lu);
                	}
                    break;
                }
                case '%':
                {
                    uart_putc('%');
                    break;
                }
                default:
                {
                    // Unhandled fallback: print raw characters
                    uart_putc('%');
                    uart_putc(*format);
                    break;
                }
            }
            format++; // Advance smoothly to the next string character
        }
    }
    __builtin_va_end(args);
}


void uart_custom_scan(const char *format, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, format);

    for(const char *scan_ptr = format; *scan_ptr != '\0'; scan_ptr++)
    {
        if(*scan_ptr == '%')
        {
            switch (*++scan_ptr)
            {
                case SCAN_POSITIVE_DECIMAL_NUMBER:
                {
                    int *val = __builtin_va_arg(args, int *);

                    // Reuse existing hardware scan driver
                    *val = (int)atoui(uart_scan_uint());
                    break;
                }
                case SCAN_NEGATIVE_DECIMAL_NUMBER:
                {
                    int *val = __builtin_va_arg(args, int *);
                    char *str = uart_scan_uint();
                    *val = 0;

                    // Logic to handle negative sign manually
                    int sign = 1;
                    if (*str == '-')
                    {
                    	sign = -1; str++;
                    }

                    *val = (int)atoui(str) * sign;
                    break;
                }
                case SCAN_CHARACTER:
                {
                    char *ptr = __builtin_va_arg(args, char *);
                    uart_getc(ptr); // Reuse existing UART string driver
                    break;
                }
            }
        }
    }
    __builtin_va_end(args);
}

