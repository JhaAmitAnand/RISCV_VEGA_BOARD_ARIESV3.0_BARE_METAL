/*
 * ARIES V3.0 / VEGA ET1031
 * 64-bit Factorial API (Double-Register Math)
 */

/* Manual type definitions to avoid stdint.h dependency */
typedef unsigned int uint32_t;         // 32-bit width
typedef unsigned long long uint64_t;    // 64-bit width (uses register pairs)

/* UART Register Definitions for ARIES V3.0 */
#define UART0_BASE 0x10000100
#define UART_THR   (*((volatile uint32_t*)(UART0_BASE + 0x00)))
#define UART_RBR   (*((volatile uint32_t*)(UART0_BASE + 0x00)))
#define UART_LSR   (*((volatile uint32_t*)(UART0_BASE + 0x14)))

#define UART_LSR_THRE 0x20  /* Transmit Holding Register Empty */
#define UART_LSR_DR   0x01  /* Data Ready */

/* --- UART DRIVERS --- */

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

char uart_getc() {
    /* Wait until a character is received */
    while (!(UART_LSR & UART_LSR_DR));
    return (char)UART_RBR;
}

/* --- 64-BIT MATH & CONVERSION API --- */

/**
 * Calculates factorial using 64-bit arithmetic.
 * On this 32-bit CPU, the compiler will use pairs of registers (e.g., a0, a1)
 * to store and calculate this value.
 */
uint64_t factorial_64(uint32_t n) {
    uint64_t res = 1;
    for (uint32_t i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

/**
 * Custom 64-bit decimal printer.
 * Since we don't have a C library, we manually extract digits.
 */
void print_dec64(uint64_t n) {
    if (n == 0) {
        uart_putc('0');
        return;
    }
    char buf[21]; // Buffer for up to 20 digits (max uint64_t)
    int i = 0;
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    /* Print digits in reverse order */
    while (--i >= 0) {
        uart_putc(buf[i]);
    }
}

/**
 * Simple string to integer helper for multi-digit input
 */
uint32_t atoui(const char* s) {
    uint32_t res = 0;
    while (*s >= '0' && *s <= '9') {
        res = res * 10 + (*s - '0');
        s++;
    }
    return res;
}

/* --- MAIN PROGRAM --- */

int main() {
    char input_buf[16];
    int idx;

    /* Success message proves memory mapping at 0x200000 is working */
    print_str("\r\n--- VEGA 64-bit Factorial Engine ---\r\n");
    print_str("Note: Max input is 20 for 64-bit results.\r\n\r\n");

    while (1) {
        print_str("Enter Number: ");
        idx = 0;

        /* Collect multi-digit input */
        while (1) {
            char c = uart_getc();

            if (c == '\r' || c == '\n') {
                input_buf[idx] = '\0';
                break;
            } else if (c == 8 || c == 127) { /* Backspace */
                if (idx > 0) {
                    idx--;
                    print_str("\b \b");
                }
            } else if (c >= '0' && c <= '9' && idx < 15) {
                input_buf[idx++] = c;
                uart_putc(c); /* Echo char */
            }
        }

        print_str("\r\n");

        if (idx > 0) {
            uint32_t num = atoui(input_buf);

            if (num > 20) {
                print_str("Error: 21! overflows even 64-bit registers.\r\n");
            } else {
                uint64_t result = factorial_64(num);
                print_str("Result: ");
                print_dec64(result);
                print_str("\r\n");
            }
        }
        print_str("\r\n");
    }

    return 0;
}
