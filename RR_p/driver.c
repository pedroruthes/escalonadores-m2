/**
 * Driver.c
 *
 * O formato do agendamento é:
 *
 *  [nome] [prioridade] [burst da CPU]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedule_rr_p.h"

#define SIZE    100

// Array de ponteiros para filas de prioridade
Task *priority_queues[MAX_PRIORITY] = {NULL};

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name; // Nome da tarefa
    int priority; // Prioridade da tarefa
    int burst; // Burst (tempo de execução) da CPU para a tarefa

    in = fopen(argv[1],"r"); // Abre o arquivo de entrada especificado na linha de comando para leitura

    // Loop para ler cada linha do arquivo    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task); // Cria uma cópia da linha lida
        name = strsep(&temp,","); // Extrai o nome da tarefa usando ',' como delimitador
        priority = atoi(strsep(&temp,",")); // Extrai e converte a prioridade para inteiro
        burst = atoi(strsep(&temp,",")); // Extrai e converte o burst da CPU para inteiro

        add(name,priority,burst); // Adiciona a tarefa à fila de prioridade
        free(temp); // Libera a memória alocada para temp
    }

    fclose(in); // Fecha o arquivo de entrada após a leitura de todas as tarefas
    schedule(); // Chama a função para realizar o escalonamento das tarefas

    return 0;
}