/* =====================================================
 *
 * File Name   : custom_threads.h
 * File Type   : Header File (C Header File)
 * File Author : Amit Anand Jha
 *
 * File Usage  : This is where the we define the custom
 * structures to spawn threads and help them to be initialized
 * created, joined back etc
 *
 * ===================================================== */

#ifndef THREAD_H
#define THREAD_H

#include "hw_drivers.h"
#include "64_bits_maths.h"



#define MAX_THREADS 8
#define STACK_SIZE  1024

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

/* RISC-V Context saved on thread stack */
typedef struct {
    uint32_t ra;
    uint32_t sp;
    uint32_t s[12]; /* Callee-saved registers s0-s11 */
} thread_context_t;

typedef struct tcb {
    int id;
    thread_state_t state;
    uint32_t *sp;
    uint32_t stack[STACK_SIZE];
    struct tcb *join_target;
} tcb_t;

void thread_init(void);
int thread_create(void (*start_routine)(void *), void *arg);
void thread_yield(void);
int thread_join(int thread_id);

#endif /* THREAD_H */
