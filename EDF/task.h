#ifndef TASK_H
#define TASK_H

// Representation of a task in the system
typedef struct task {
    char *name;
    int priority;
    int burst;
    int deadline;
    struct task *next;
} Task;

#endif
