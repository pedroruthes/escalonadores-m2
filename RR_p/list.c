/**
 * Various list operations
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"


// Adiciona uma nova tarefa à lista de tasks
void insert(struct node **head, Task *newTask) {
    // Aloca memória para um novo nó da lista
    struct node *newNode = malloc(sizeof(struct node));

    // Define a tarefa do novo nó como a task fornecida
    newNode->task = newTask;
    // O próximo nó aponta para o head atual da lista
    newNode->next = *head;
    // O head é atualizado para apontar para o novo nó
    *head = newNode;
}

// Deleta a tarefa selecionada da lista
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    // Inicializa temp como a cabeça da lista
    temp = *head;

    // Caso especial - início da lista
    if (strcmp(task->name, temp->task->name) == 0) {
        *head = (*head)->next; // Atualiza o head para o próximo nó
    } else {
        // Interior ou último elemento da lista
        prev = *head;
        temp = temp->next;

        // Enquanto não encontrarmos a tarefa a ser removida, avançamos na lista
        while (strcmp(task->name, temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        // Atualiza o ponteiro next do nó anterior para pular o nó a ser removido
        prev->next = temp->next;
    }
}

// Percorre e imprime os elementos da lista
void traverse(struct node *head) {
    struct node *temp;
    temp = head; // Inicializa temp com o head da lista

    // Enquanto temp não for nulo, ou seja, ainda há elementos na lista
    while (temp != NULL) {
        // Imprime os detalhes da tarefa do nó atual
        printf("[%s] [%d] [%d]\n", temp->task->name, temp->task->priority, temp->task->burst);
        // Avança para o próximo nó na lista
        temp = temp->next;
    }
}
