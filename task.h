/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// Representação da Task
typedef struct task {
    char *name;
    int priority;
    int burst;
    struct task *next;    // Campo para o próximo ponteiro na lista
} Task;

#endif
