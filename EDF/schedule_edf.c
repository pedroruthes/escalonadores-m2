#include "schedule_edf.h"
#include "list.h"
#include "task.h"
#include "CPU.h"
#include <stdlib.h>
#include <string.h>  // Adicione este cabeçalho

struct node *head = NULL; // Declare the head of the list

// add a task to the list 
void add(char *name, int priority, int burst, int deadline) {
    // Create a new task
    Task *newTask = (Task *) malloc(sizeof(Task));
    newTask->name = strdup(name);  // Use strdup to avoid issues with the task name
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->deadline = deadline;

    // Insert the task into the list of ready tasks
    insert(&head, newTask);
}

// invoke the scheduler
void schedule() {
    // Continue scheduling until no tasks are left
    while (!isEmpty(head)) {
        // Find the task with the earliest deadline
        Task *earliest = NULL;
        struct node *temp = head;
        while (temp != NULL) {
            Task *task = temp->task;
            if (earliest == NULL || task->deadline < earliest->deadline) {
                earliest = task;
            }
            temp = temp->next;
        }

        // Execute the task with the earliest deadline
        if (earliest != NULL) {
            run(earliest, earliest->burst);
            delete(&head, earliest);
        }
    }
}
