/* =====================================================
 *
 * File Name   : main.c
 * File Type   : Source File (C)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is where the linker comes to pick the 'main'
 * function
 * ===================================================== */

#include "main.h"

void uart_putc(char c) {
    /* Wait for Transmit Holding Register to be empty */
    while (!(UART_LSR & UART_LSR_THRE));
    UART_THR = c;
}

void print_str(const char* s) {
    while (*s != '\0') {
        uart_putc(*s++);
    }
}


/* Defining the delay API which can basically help to insert delays */
void delay(volatile int count) {
    while(count--) {

    	DO_NOTHING_CONSUME_A_CYCLE;
    }
}


int main() {
    // Keep it extremely simple: no extra functions, just the loop.
    while (1) {
    	UART_THR = '\n';
        delay(10000);
        UART_THR = 'N';
        delay(10000);
        UART_THR = 'a';
        delay(10000);
        UART_THR = 'm';
        delay(10000);
        UART_THR = 'a';
        delay(10000);
        UART_THR = 's';
        delay(10000);
        UART_THR = 't';
        delay(10000);
        UART_THR = 'e';

        delay(10000);
        UART_THR = '\t';
        delay(10000);

        UART_THR = 'W';
        delay(10000);
        UART_THR = 'o';
        delay(10000);
        UART_THR = 'r';
        delay(10000);
        UART_THR = 'l';
        delay(10000);
        UART_THR = 'd';
        delay(10000);

        UART_THR = '\n';
        delay(10000);
        UART_THR = '\r';

        print_str("\nNamaste World above was printed by writing each alphabet directly on the register address");
        print_str("\n\n");
    }
    return 0;
}
