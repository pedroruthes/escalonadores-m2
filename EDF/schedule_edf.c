#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CPU.h"
#include <pthread.h>
#include <unistd.h>

#define TIME_QUANTUM 10  // Define o tempo máximo de execução para cada parte de tempo

// Variável global para sinalizar o fim de uma parte de tempo
volatile int time_slice_flag = 0;

// Função de simulação de timer
void* timer_simulation(void* arg) {
    while (1) {
        sleep(1);  // Simula a duração de uma parte de tempo (1 segundo para simplicidade)
        time_slice_flag = 1;  // Sinaliza que uma parte de tempo terminou
    }
    return NULL;
}

// Função auxiliar para criar uma nova tarefa
Task* create_task(char* name, int priority, int burst, int deadline) {
    Task* new_task = (Task*)malloc(sizeof(Task));
    new_task->name = strdup(name);  // Copia o nome da tarefa
    new_task->priority = priority;  // Define a prioridade
    new_task->burst = burst;  // Define o tempo de burst
    new_task->deadline = deadline;  // Define a deadline
    new_task->next = NULL;  // Inicializa o próximo ponteiro como NULL
    return new_task;
}

// Adiciona uma tarefa à lista, inserindo-a com base na deadline
void add(char* name, int priority, int burst, int deadline) {
    Task* new_task = create_task(name, priority, burst, deadline);
    Task** queue = &priority_queues[priority];  // Obtém a fila de tarefas para a prioridade dada

    // Insere a tarefa na posição correta com base na deadline
    if (*queue == NULL || (*queue)->deadline > deadline) {
        new_task->next = *queue;
        *queue = new_task;
    } else {
        Task* temp = *queue;
        while (temp->next != NULL && temp->next->deadline <= deadline) {
            temp = temp->next;
        }
        new_task->next = temp->next;
        temp->next = new_task;
    }
}

// Função auxiliar para obter a próxima tarefa a ser executada
Task* get_next_task() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        if (priority_queues[i] != NULL) {
            Task* task = priority_queues[i];  // Obtém a tarefa da frente da fila
            priority_queues[i] = task->next;  // Atualiza a fila removendo a tarefa obtida
            task->next = NULL;
            return task;
        }
    }
    return NULL;  // Retorna NULL se não houver tarefas
}

// Invoca o escalonador
void schedule() {
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, timer_simulation, NULL);  // Cria uma thread para a simulação do timer
    pthread_detach(timer_thread);  // Desvincula a thread para que ela rode em segundo plano

    int time = 0;  // Inicializa o tempo total de execução
    Task* task = get_next_task();  // Obtém a primeira tarefa a ser executada

    while (task != NULL) {
        while (task->burst > 0 && !time_slice_flag) {
            // Espera até que a flag da parte de tempo seja setada
            usleep(1000);  // Espera por 1 milissegundo
        }
        if (time_slice_flag) {
            time_slice_flag = 0;  // Reseta a flag

            int run_time = (task->burst < TIME_QUANTUM) ? task->burst : TIME_QUANTUM;  // Calcula o tempo de execução
            int new_time = time + run_time;  // Atualiza o tempo total de execução

            if (new_time > task->deadline) {
                // Verifica se a tarefa não pode ser completada antes da deadline
                printf("Task %s não conseguiu completar antes da deadline.\n", task->name);
                free(task->name);
                free(task);
            } else {
                run(task, run_time);  // Executa a tarefa pelo tempo calculado
                printf("Tempo: %d\n", time);
                time = new_time;
                task->burst -= run_time;  // Atualiza o tempo de burst restante

                printf("Task %s - Burst restante: %d\n", task->name, task->burst);

                if (task->burst <= 0) {
                    // Se a tarefa estiver completa
                    printf("Task %s finalizada.\n", task->name);
                    free(task->name);
                    free(task);
                } else {
                    // Reinsere a tarefa na fila se ainda houver tempo de burst restante
                    add(task->name, task->priority, task->burst, task->deadline);
                }
            }
        }

        task = get_next_task();  // Obtém a próxima tarefa a ser executada
    }
}