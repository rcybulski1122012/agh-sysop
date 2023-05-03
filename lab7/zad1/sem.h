#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int createSemaphore(char* pathname, int semuid, int initVal);
int getSemaphoreId(char* pathname, int semuid);
int incrementSemaphoreValue(int semId);
int incrementSemaphoreVal(int semId, bool nowait);
int decrementSemaphoreValue(int semId);
int decrementSemaphoreVal(int semId, bool nowait);
int getSemaphoreVal(int semId);
int destroySemaphore(int semId);