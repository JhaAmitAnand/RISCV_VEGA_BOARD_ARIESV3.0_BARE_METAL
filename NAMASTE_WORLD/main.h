/* =====================================================
 *
 * File Name   : main.h
 * File Type   : Header File (C Header File)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is where the we define the ARIESV3.0
 * board HW specific details which will be extremely useful
 * to program the board. Details like Pin Addresses, Baud rate
 * Register Addresses will be defined here
 *
 * ===================================================== */

#ifndef MAIN_H
#define MAIN_H


/* ====================================================================
 *                             MACROS
 * ==================================================================== */

/* The baud rate (bits per second) at which communication with ARIESV3.0 needs to happen */
#define BAUD_RATE                     115200


/* Address of the Physical UART controller on the ARIESV3.0 board*/
#define UART0_BASE                    0x10000100

/* Defining Address to the THR (TRANSMITTER HOLDING REGISTER) */
#define UART_THR                      (*(volatile unsigned int*)(UART0_BASE + 0x00))

/* Defining Address to the LSR (LINE STATUS REGISTER) */
#define UART_LSR                      (*(volatile unsigned int*)(UART0_BASE + 0x14))

/* Transmit Holding Register Empty - the 5th bit of LSR register
 * IF UART_LSR_THRE = 0 => Transmit Register is still busy sending previous character
 * IF UART_LSR_THRE = 1 => THR Register is empty and new character can be sent */
#define UART_LSR_THRE                 0x20


/* Each NOP takes one cycle. Its a valid INSTRUCTION for compiler */
#define DO_NOTHING_CONSUME_A_CYCLE    __asm__ volatile ("nop")



/* ====================================================================
 *                      FUNCTUION DECLARATIONS
 * ==================================================================== */
/* Declaring the delay API which can basically help to insert delays */
void delay(volatile int count);
void uart_putc(char c);
void print_str(const char* s);


#endif /* MAIN_H_ */
