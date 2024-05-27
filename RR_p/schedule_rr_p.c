#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CPU.h"
#include <pthread.h>
#include <unistd.h>

#define TIME_QUANTUM 10 // Definição do tempo do quantum

// Variável global para sinalizar o fim do time slice
volatile int time_slice_flag = 0;

// Função de simulação do temporizador
void* timer_simulation(void* arg) {
    while (1) {
        sleep(1);  // Simula a duração do time slice (0.1 segundo para simplicidade)
        time_slice_flag = 1; // Sinaliza que o time slice terminou
    }
    return NULL;
}

// Função auxiliar para criar uma nova task
Task* create_task(char* name, int priority, int burst) {
    Task* new_task = (Task*)malloc(sizeof(Task)); // Aloca memória para a nova tarefa
    new_task->name = strdup(name); // Copia o nome da tarefa
    new_task->priority = priority; // Define a prioridade da tarefa
    new_task->burst = burst; // Define o burst da CPU para a tarefa
    new_task->next = NULL; // Inicializa o próximo ponteiro como NULL

    return new_task; // Retorna o ponteiro para a nova tarefa
}

// Adiciona uma task à lista
void add(char* name, int priority, int burst) {
    Task* new_task = create_task(name, priority, burst); // Cria uma nova tarefa
    Task** queue = &priority_queues[priority]; // Obtem a fila correspondente à prioridade

    // Adiciona a nova tarefa à fila
    if (*queue == NULL) {
        *queue = new_task; // Se a fila está vazia, a nova tarefa é a primeira
    } else {
        Task* temp = *queue;
        while (temp->next != NULL) { // Percorre até o final da fila
            temp = temp->next;
        }
        temp->next = new_task; // Adiciona a nova tarefa ao final da fila
    }
}

// Função auxiliar para obter a próxima task a ser executada
Task* get_next_task() {
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        if (priority_queues[i] != NULL) {
            Task* task = priority_queues[i]; // Obtém a primeira tarefa da fila
            priority_queues[i] = task->next; // Atualiza a fila para apontar para a próxima tarefa
            task->next = NULL; // Desconecta a tarefa da fila
            return task; // Retorna a tarefa
        }
    }
    return NULL; // Retorna NULL se não houver mais tarefas
}

// Invoca o escalonador
void schedule() {
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, timer_simulation, NULL); // Cria um thread para a simulação do temporizador
    pthread_detach(timer_thread); // Desanexa o thread

    int time = 0; // Inicializa o tempo total de execução
    Task* task = get_next_task(); // Obtém a primeira tarefa a ser executada

    while (task != NULL) {
        while (task->burst > 0 && !time_slice_flag) {
            // Espera até que o time slice termine
            usleep(1000); // Aguarda 1 milissegundo
        }
        if (time_slice_flag) {
            time_slice_flag = 0;  // Reseta o flag do time slice

            int run_time = task->burst > TIME_QUANTUM ? TIME_QUANTUM : task->burst; // Calcula o tempo de execução
            task->burst -= run_time; // Atualiza o burst da tarefa

            printf("Tempo: %d\n", time);
            run(task, run_time); // Printa a task

            time += run_time; // Atualiza o tempo total de execução

            if (task->burst > 0) {
                // Se a tarefa não terminou, adiciona de volta à fila
                add(task->name, task->priority, task->burst);
                free(task->name); // Libera o nome da tarefa
                free(task); // Libera a memória da tarefa
            } else {
                // Se a tarefa terminou, imprime a mensagem e libera a memória
                printf("Task %s finalizada.\n", task->name);
                free(task->name); // Libera o nome da tarefa
                free(task); // Libera a memória da tarefa
            }

            task = get_next_task(); // Obtém a próxima tarefa a ser executada
        }
    }
}