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
#include "custom_threads.h"

void thread_1(void* arg) {
    while(1) {
        uart_custom_print("Thread 1: %s\r\n", (char*)arg);
        delay(500000);
        thread_yield();
    }
}

void thread_2(void* arg) {
    while(1) {
        uart_custom_print("Thread 2: 5! = %lu\r\n", factorial_64(5));
        delay(500000);
        thread_yield();
    }
}

void thread_3(void* arg) {
    while(1) {
        uart_custom_print("Thread 3: System Alive\r\n");
        delay(500000);
        thread_yield();
    }
}

// Separate context for the primary thread background runner
void scheduler_runner(void) {
    while(1) {
        thread_yield();
    }
}

int main() {
    print_vega_startup_design();

    thread_init();

    thread_create(thread_1, "Hello from T1");
    thread_create(thread_2, NULL);
    thread_create(thread_3, NULL);

    scheduler_runner();

    return 0;
}
