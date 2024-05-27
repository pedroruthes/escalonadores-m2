/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// Representação da Task
typedef struct task {
    char *name;       // Nome da tarefa
    int priority;     // Prioridade da tarefa
    int burst;        // Tempo de burst (duração de execução) da tarefa
    struct task *next; // Ponteiro para a próxima tarefa na lista
} Task;

#endif // TASK_H
