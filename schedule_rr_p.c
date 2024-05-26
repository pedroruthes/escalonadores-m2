#include "schedulers.h"
#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Incluir a biblioteca unistd.h para usleep

#define TIME_QUANTUM 1

pthread_t timer_thread;
int time_overflow = 0;
int slice_end_flag = 0;

Task *priority_queues[MAX_PRIORITY] = {NULL};

Task *create_task(char *name, int priority, int burst) {
    Task *new_task = (Task *)malloc(sizeof(Task));
    new_task->name = strdup(name);
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->next = NULL;
    return new_task;
}

void add(char *name, int priority, int burst) {
    Task *new_task = create_task(name, priority, burst);
    Task **queue = &priority_queues[priority];
    
    if (*queue == NULL) {
        *queue = new_task;
    } else {
        Task *temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_task;
    }
}

Task *get_next_task() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        if (priority_queues[i] != NULL) {
            Task *task = priority_queues[i];
            priority_queues[i] = task->next;
            task->next = NULL;
            return task;
        }
    }
    return NULL;
}

void *timer_function(void *arg) {
    int *time_ptr = (int *)arg;
    while (1) {
        usleep(10000);  // Simula o tempo passando (10ms)
        (*time_ptr)++;
        if (*time_ptr >= TIME_QUANTUM) {
            time_overflow = 1;
            slice_end_flag = 1;
            *time_ptr = 0;
        }
    }
    return NULL;
}

void schedule() {
    int time = 0;
    Task *task = get_next_task();
    
    pthread_create(&timer_thread, NULL, timer_function, (void *)&time);

    while (task != NULL) {
        run(task, TIME_QUANTUM);
        
        if (time_overflow) {
            slice_end_flag = 0;
            time_overflow = 0;
            add(task->name, task->priority, task->burst);
            free(task->name);
            free(task);
            task = get_next_task();
        }

        int run_time = task->burst > TIME_QUANTUM ? TIME_QUANTUM : task->burst;
        task->burst -= run_time;
        time += run_time;
        
        printf("Time: %d\n", time);
        
        if (task->burst > 0) {
            add(task->name, task->priority, task->burst);
            free(task->name);
            free(task);
        } else {
            printf("Task %s finalizada.\n", task->name);
            free(task->name);
            free(task);
        }
        
        task = get_next_task();
    }

    pthread_join(timer_thread, NULL);
}
