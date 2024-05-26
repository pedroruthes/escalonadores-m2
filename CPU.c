/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

// run this task for the specified time slice
void run(Task *task, int slice) {
    printf("Task em execução = [%s] [%d] [%d] por %d u.t\n",task->name, task->priority, task->burst, slice);
}
