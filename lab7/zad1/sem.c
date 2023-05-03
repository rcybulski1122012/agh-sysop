#include "sem.h"

int createSemaphore(char* pathname, int semuid, int initVal) {
    key_t semKey = ftok(pathname, semuid);
    if(semKey == -1) {
        perror("createSemaphore: ftok");
        return -1;
    }

    int semId = semget(semKey, 1, 0666 | IPC_CREAT);
    if (semId == -1){
        perror("createSemaphore: semget");
        return -1;
    }

    int result = semctl(semId, 0, SETVAL, initVal);
    if(result == -1){
        perror("createSemaphore: semctl");
        return -1;
    }

    return semId;
}


int getSemaphoreId(char* pathname, int semuid) {
    key_t semKey = ftok(pathname, semuid);
    if(semKey == -1) {
        perror("createSemaphore: ftok");
        return -1;
    }

    int semId = semget(semKey, 1, 0);
    if (semId == -1){
        perror("createSemaphore: semget");
        return -1;
    }

    return semId;
}


int incrementSemaphoreVal(int semId, bool nowait) {
    struct sembuf semBuf;
    semBuf.sem_flg = nowait ? IPC_NOWAIT : 0;
    semBuf.sem_num = 0;
    semBuf.sem_op = 1;

    return semop(semId, &semBuf, 1);
}

int incrementSemaphoreValue(int semId) {
    return incrementSemaphoreVal(semId, false);
}

int decrementSemaphoreVal(int semId, bool nowait) {
    struct sembuf semBuf;
    semBuf.sem_flg = nowait ? IPC_NOWAIT : 0;
    semBuf.sem_num = 0;
    semBuf.sem_op = -1;

    return semop(semId, &semBuf, 1);
}

int decrementSemaphoreValue(int semId) {
    return decrementSemaphoreVal(semId, false);
}


int getSemaphoreVal(int semId) {
    return semctl(semId, 0, GETVAL);
}

int destroySemaphore(int semId) {
    int result = semctl(semId, 0, IPC_RMID);

    if(result == -1) {
        perror("destroySemaphore: semctl");
    }

    return result;
}

