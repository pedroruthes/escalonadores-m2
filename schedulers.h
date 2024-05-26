#ifndef SCHEDULERS_H
#define SCHEDULERS_H

// Define as prioridades mínimas e máximas
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// Incluí a biblioteca task.h (por conta da fila de prioridades) e pthread para lidar com threads
#include "task.h"
#include <pthread.h> 

// Cria um conjunto de filas de prioridade
extern Task *priority_queues[MAX_PRIORITY];

// Adiciona a task na lista
void add(char *name, int priority, int burst);

// Invoca o escalonador Round-Robin por prioridade
void schedule();

#endif // SCHEDULERS_H