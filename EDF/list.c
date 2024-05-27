#include <stdlib.h>
#include "list.h"

// Insere uma nova task na lista
void insert(struct node **head, Task *task) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    newNode->task = task;
    newNode->next = *head;
    *head = newNode;
}

// Remove uma task da lista
void delete(struct node **head, Task *task) {
    struct node *temp = *head, *prev = NULL;

    // Se a task a ser deletada está na cabeça
    if (temp != NULL && temp->task == task) {
        *head = temp->next;
        free(temp);
        return;
    }

    // Procura a task a ser deletada
    while (temp != NULL && temp->task != task) {
        prev = temp;
        temp = temp->next;
    }

    // Se a task não foi encontrada
    if (temp == NULL) return;

    // Desconecta a task da lista e libera memória
    prev->next = temp->next;
    free(temp);
}

// Verifica se a lista está vazia
int isEmpty(struct node *head) {
    return head == NULL;
}
