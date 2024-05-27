#include "CPU.h"
#include <stdio.h>

void run(Task *task, int slice) {
    printf("Running task = [%s] [Priority = %d] [Burst Time = %d] [Deadline = %d]\n",
           task->name, task->priority, task->burst, task->deadline);
}
