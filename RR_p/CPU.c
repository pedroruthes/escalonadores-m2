/**
 * Esta é uma "CPU virtual" que também mantém o controle do tempo do sistema.
*/

#include <stdio.h>

#include "task.h"

// Printa esta tarefa pelo intervalo de tempo especificado
void run(Task *task, int slice) {
    printf("Task em execução = [%s] [%d] [%d] por %d u.t.\n",task->name, task->priority, task->burst, slice);
}
