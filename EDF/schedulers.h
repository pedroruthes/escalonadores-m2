#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "task.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// Fila de aptos global para diferentes prioridades
extern Task *priority_queues[MAX_PRIORITY];

// Function declarations
void add(char* name, int priority, int burst, int deadline);
void schedule();

#endif // SCHEDULERS_H
