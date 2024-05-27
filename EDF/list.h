/**
 * Estrutura de dados da lista contendo as tarefas no sistema
 */

#include "task.h"

// Definição da estrutura do nó da lista
struct node {
    Task *task; // Ponteiro para a tarefa armazenada no nó
    struct node *next; // Ponteiro para o próximo nó na lista
};

// Declarações das operações de inserção e exclusão
void insert(struct node **head, Task *task); // Função para inserir uma nova tarefa na lista
void delete(struct node **head, Task *task); // Função para deletar uma tarefa da lista
void traverse(struct node *head); // Função para percorrer e imprimir os elementos da lista