/* =====================================================
 * File Name   : custom_threads.c
 * File Type   : Source File (C)
 * File Author : Amit Anand Jha
 *
 * File Usage  : Thread manager execution and stack initialization.
 * ===================================================== */


#include "custom_threads.h"

static tcb_t threads[MAX_THREADS];
static int current_thread = -1;
static int thread_count = 0;

extern void context_switch(uint32_t **old_sp, uint32_t *new_sp);
extern void thread_start_stub(void);

void thread_exit_wrapper(void (*routine)(void *), void *arg) {
    routine(arg);
    threads[current_thread].state = THREAD_TERMINATED;

    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state == THREAD_BLOCKED &&
            threads[i].join_target == &threads[current_thread]) {
            threads[i].state = THREAD_READY;
            threads[i].join_target = NULL;
        }
    }
    thread_yield();
}

void thread_init(void) {
    threads[0].id = 0;
    threads[0].state = THREAD_RUNNING;
    threads[0].join_target = NULL;

    // Offset array bounds to avoid collisions when saving main context register state
    uint32_t *sp = &threads[0].stack[STACK_SIZE - 16];
    threads[0].sp = (uint32_t *)((uint32_t)sp & ~15UL);

    current_thread = 0;
    thread_count = 1;
}

int thread_create(void (*start_routine)(void *), void *arg) {
    if (thread_count >= MAX_THREADS) return -1;

    int id = thread_count++;
    tcb_t *t = &threads[id];
    t->id = id;
    t->state = THREAD_READY;
    t->join_target = NULL;

    uint32_t *sp = &t->stack[STACK_SIZE];

    sp -= 16;
    sp = (uint32_t *)((uint32_t)sp & ~15UL);

    sp[0]  = (uint32_t)thread_start_stub;
    sp[1]  = (uint32_t)start_routine;
    sp[2]  = (uint32_t)arg;

    for (int i = 3; i < 16; i++) {
        sp[i] = 0;
    }

    t->sp = sp;
    return id;
}

void thread_yield(void) {
    if (thread_count <= 1) return;

    int prev = current_thread;
    int next = (current_thread + 1) % thread_count;

    while (threads[next].state != THREAD_READY && next != prev) {
        next = (next + 1) % thread_count;
    }

    if (next == prev) return;

    if (threads[prev].state == THREAD_RUNNING) {
        threads[prev].state = THREAD_READY;
    }
    threads[next].state = THREAD_RUNNING;
    current_thread = next;

    context_switch(&threads[prev].sp, threads[next].sp);
}
