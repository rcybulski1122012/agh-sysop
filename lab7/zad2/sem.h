#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

sem_t* createSemaphore(char* name, int initVal);
sem_t* getSemaphore(char* name);
