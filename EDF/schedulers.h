#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "task.h"

#define MIN_PRIORITY 1 // Define a prioridade mínima como 1
#define MAX_PRIORITY 10 // Define a prioridade máxima como 10

// Declaração de uma fila global de aptos para diferentes prioridades
extern Task *priority_queues[MAX_PRIORITY];

#endif // SCHEDULERS_H
