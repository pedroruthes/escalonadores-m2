#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CPU.h"
#include <pthread.h>
#include <unistd.h>

#define TIME_QUANTUM 10

// Global variables
volatile int time_slice_flag = 0;
int global_time = 0;  // Global time counter

// Timer simulation function
void* timer_simulation(void* arg) {
    while (1) {
        sleep(1);  // Simulate the time slice duration (1 second for simplicity)
        time_slice_flag = 1;
        global_time++;
    }
    return NULL;
}

// Helper function to create a new task
Task* create_task(char* name, int priority, int burst, int deadline) {
    Task* new_task = (Task*)malloc(sizeof(Task));
    new_task->name = strdup(name);
    new_task->priority = priority;
    new_task->burst = burst;
    new_task->deadline = deadline;
    new_task->next = NULL;
    return new_task;
}

// Add a task to the list
void add(char* name, int priority, int burst, int deadline) {
    Task* new_task = create_task(name, priority, burst, deadline);
    Task** queue = &priority_queues[priority];

    if (*queue == NULL) {
        *queue = new_task;
    } else {
        Task* temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_task;
    }
}

// Helper function to get the next task to run
Task* get_next_task() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        if (priority_queues[i] != NULL) {
            Task* task = priority_queues[i];
            priority_queues[i] = task->next;
            task->next = NULL;
            return task;
        }
    }
    return NULL;
}

// Invoke the scheduler
void schedule() {
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, timer_simulation, NULL);
    pthread_detach(timer_thread);

    Task* task = get_next_task();

    while (task != NULL) {
        while (task->burst > 0 && !time_slice_flag) {
            // Wait for the time slice flag to be set
            usleep(1000);
        }
        if (time_slice_flag) {
            time_slice_flag = 0;  // Reset the flag

            int run_time = (task->burst < TIME_QUANTUM) ? task->burst : TIME_QUANTUM;
            run(task, run_time);
            task->burst -= run_time;

            printf("Task %s executed for %d units. Remaining burst: %d. Global time: %d\n", task->name, run_time, task->burst, global_time);

            // Check if the task missed its deadline
            if (global_time > task->deadline) {
                printf("Task %s missed its deadline. Deadline: %d, Global time: %d\n", task->name, task->deadline, global_time);
            }

            if (task->burst <= 0) {
                free(task->name);
                free(task);
            } else {
                // Re-add the task to the queue if it still has burst time remaining
                add(task->name, task->priority, task->burst, task->deadline);
            }
        }

        task = get_next_task();
    }
}
