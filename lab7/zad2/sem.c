#include "sem.h"

sem_t* createSemaphore(char* name, int initVal) {
    sem_t* semaphore = sem_open(name, O_CREAT | O_EXCL, 0666, initVal);
    if (semaphore == SEM_FAILED) {
        perror("createSemaphore: sem_open");
        return NULL;
    }

    return semaphore;
}

sem_t* getSemaphore(char* name) {
    sem_t* semaphore = sem_open(name, 0);
    if(semaphore == SEM_FAILED) {
        perror("getSemaphore: sem_open");
        return NULL;
    }

    return semaphore;
}
