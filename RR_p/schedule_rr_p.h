#define MIN_PRIORITY 1 // Define a prioridade mínima como 1
#define MAX_PRIORITY 10 // Define a prioridade máxima como 10

// Função para adicionar uma tarefa à lista
void add(char *name, int priority, int burst);

// Função para invocar o escalonador
void schedule();
