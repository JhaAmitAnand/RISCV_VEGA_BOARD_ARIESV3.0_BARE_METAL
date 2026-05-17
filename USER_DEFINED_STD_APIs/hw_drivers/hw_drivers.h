/* =====================================================
 *
 * File Name   : hw_drivers.h
 * File Type   : Header File (C Header File)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is where the we define the ARIESV3.0
 * board HW specific details which will be extremely useful
 * to program the board. Details like Pin Addresses, Baud rate
 * Register Addresses will be defined here
 *
 * ===================================================== */

#ifndef HW_DRIVERS
#define HW_DRIVERS


/* ====================================================================
 *                             MACROS
 * ==================================================================== */

/* The baud rate (bits per second) at which communication with ARIESV3.0 needs to happen */
#define BAUD_RATE                     115200


/* Address of the Physical UART controller on the ARIESV3.0 board*/
#define UART0_BASE                    0x10000100

/* Defining Address to the THR (TRANSMITTER HOLDING REGISTER) :  A WRITE ONLY REGISTER */
#define UART_THR                      (*(volatile unsigned int*)(UART0_BASE + 0x00))

/* Defining Address to the RBR (RECEIVER BUFFER REGISTER) :  A READ ONLY REGISTER */
#define UART_RBR                      (*(volatile unsigned int*)(UART0_BASE + 0x00))

/* Defining Address to the LSR (LINE STATUS REGISTER) */
#define UART_LSR                      (*(volatile unsigned int*)(UART0_BASE + 0x14))


/* Transmit Holding Register Empty - the 5th bit of LSR register
 * IF UART_LSR_THRE = 0 => Transmit Register is still busy sending previous character
 * IF UART_LSR_THRE = 1 => THR Register is empty and new character can be sent
 *
 +------------+---------------------+-----------------+---------------------------+
 |Action      |  Register Checked   |    Bit Used     |         Logic             |
 +------------+---------------------+-----------------+---------------------------+
 |Reading     |      UART_LSR       |    0x01 (DR)    |   If 1, call uart_getc()  |
 +------------+---------------------+-----------------+---------------------------+
 |Writing     |      UART_LSR       |    0x20 (THRE)  |   If 1, call uart_putc()  |
 +------------+---------------------+-----------------+---------------------------+  */
#define UART_LSR_THRE                 0x20
#define UART_LSR_DR                   0x01


/* Each NOP takes one cycle. Its a valid INSTRUCTION for compiler */
#define DO_NOTHING_CONSUME_A_CYCLE    __asm__ volatile ("nop")

#define MAX_NUM_CHARS_IN_USER_INPUT   32


/* The below MACROs are used to print new line or move to new lines
 * The input argument to the MACRO basically tells how many new lines
 * to be printed. The cursor starts after those many new lines
 */

#define LINE_BREAK_STRING             "\r\n"

#define NEXT_XLINE(x)                 do \
                                      {\
	                                      uint8_t temp_x = x;\
	                                      while(temp_x > 0)\
										  {\
                                    	      print_string(LINE_BREAK_STRING);\
                                    	      temp_x= temp_x-1;\
										  }\
                                      }while(0)\


/* Mapping for Maximum number of digits of a decimal number which
 * can be formed by number of bits allowed or available  */
#define MAX_NUM_DIGITS_IN_4_BITS           2    // (0-15)
#define MAX_NUM_DIGITS_IN_8_BITS           3    // (0-255)
#define MAX_NUM_DIGITS_IN_16_BITS          5    // (0-65535)
#define MAX_NUM_DIGITS_IN_32_BITS          10   // (0-4294967295)
#define MAX_NUM_DIGITS_IN_64_BITS          20   // (0-18446744073709551615)


// Define the value of NULL which would be used to initialize the pointers safely
#define NULL                               ((void*)0xFFFFFFFF)


/* ====================================================================
 *                              ENUMS
 * ==================================================================== */
typedef enum
{
	PRINT_CHARACTER = 'c',
	PRINT_STRING = 's',

	PRINT_POSITIVE_DECIMAL_NUMBER = 'u',
	PRINT_UNSIGNED_DECIMAL_NUMBER = PRINT_POSITIVE_DECIMAL_NUMBER,

	PRINT_POSITIVE_HEX_NUMBER = 'x',
	PRINT_UNSIGNED_HEX_NUMBER = PRINT_POSITIVE_HEX_NUMBER,

	PRINT_POSITIVE_OCT_NUMBER = 'o',
	PRINT_UNSIGNED_OCT_NUMBER = PRINT_POSITIVE_OCT_NUMBER,

	PRINT_POSITIVE_BINARY_NUMBER = 'b',
	PRINT_UNSIGNED_BINARY_NUMBER = PRINT_POSITIVE_BINARY_NUMBER,

	PRINT_NEGATIVE_DECIMAL_NUMBER = 'i',
	PRINT_SIGNED_DECIMAL_NUMBER = PRINT_NEGATIVE_DECIMAL_NUMBER,

	PRINT_NEGATIVE_HEX_NUMBER = 'x',
	PRINT_SIGNED_HEX_NUMBER = PRINT_NEGATIVE_HEX_NUMBER,

	PRINT_NEGATIVE_OCT_NUMBER = 'o',
	PRINT_SIGNED_OCT_NUMBER = PRINT_NEGATIVE_OCT_NUMBER,

	PRINT_NEGATIVE_BINARY_NUMBER = 'b',
	PRINT_SIGNED_BINARY_NUMBER = PRINT_NEGATIVE_BINARY_NUMBER,

	PRINT_FRACTION = 'f'
}print_format_specifier_type_e;



/* ====================================================================
 *                            TYPEDEFS
 * ==================================================================== */

/* Manual type definitions to avoid stdint.h dependency */
typedef unsigned char       uint8_t;          // 8-bit width unsigned integer (Range: 0 to 255)
typedef signed char         int8_t;          // 7-bit width signed integer and 1 bit (MSB) for sign (-127 to 0)



/* 16-bit width integer (Range: 0 to 65,535) : takes 1 LOAD SHIFT STORE cycle
 * Needs to be 2 BYTES aligned. It is precisely one single 16 bit space
 */
typedef unsigned short      uint16_t1;
typedef signed short        int16_t1;

/* 16-bit width integer (Range: 0 to 65,535) : takes 2 LOAD SHIFT STORE cycle
 * Need not be 2 BYTES aligned. It is actually two 8 bits space combined to form
 * 16 bits. Here however, we need to manually do operations accounting for
 * 2 separate bytes - carry etc to be taken care of for each byte
 */
typedef struct
{
	uint8_t lob;   // Lower Byte
	uint8_t hib;   // Higher Byte
} uint16_t2;


/* A word/halfword is HW architecture dependent definition. On ARIESV3.0 data bus is
 * 32 bits. So a word is 32 bits and a half word is 16 bits */
typedef uint16_t1           halfword_t1;
typedef uint16_t2           halfword_t2;


typedef unsigned int        uint32_t;         // 32-bit width  (uses a single register)
typedef signed int          int32_t;          // 31-bit width integer with MSB bit as sign bit

typedef unsigned long long  uint64_t;         // 64-bit width  (uses register pairs)
typedef signed long long    int_64_t;


/* A word/halfword is HW architecture dependent definition. On ARIESV3.0 data bus is
 * 32 bits. So a word is 32 bits and a Double Word (dword) is 64 bits */
typedef unsigned char       byte_t;
typedef unsigned int        word_t;           // 32-bit width  (uses a single register)
typedef unsigned long long  dword_t;          // 64-bit width  (uses register pairs)


typedef struct
{
    uint32_t v[3];       // 96-bit width (Uses 3 registers (3 * 32 = 96))
} uint96_t;


typedef struct
{
    uint32_t v[4];       // 128-bit width (Uses 4 registers (4 * 32 = 128))
} uint128_t;


typedef struct
{
    uint32_t v[8];       // 256-bit width (Uses 8 registers (8 * 32 = 256))
} uint256_t;





/* ====================================================================
 *                      FUNCTION DECLARATIONS
 * ==================================================================== */
/* Declaring the delay API which can basically help to insert delays */
void delay(volatile int count);
void uart_putc(char c);
void print_string(const char* s);
uint32_t atoui(const char* s);
char* uart_scan_uint(void);

void uart_custom_print(const char *format, ...);
void print_vega_startup_design();



#endif /* HW_DRIVERS */
