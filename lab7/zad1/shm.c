#include "shm.h"

int createSharedMemory(char* pathname, int size) {
    key_t shmKey = ftok(pathname, PROJ_ID);
    if (shmKey == -1) {
        perror("createSharedMemory: ftok");
        return -1;
    }

    int shmId = shmget(shmKey, size, IPC_CREAT | 0666);
    if(shmId == -1) {
        perror("createSharedMemory: shmget");
        return -1;
    }

    return shmId;
}

int getSharedMemoryId(char* pathname, int size) {
    key_t shmKey = ftok(pathname, PROJ_ID);
    if (shmKey == -1) {
        perror("createSharedMemory: ftok");
        return -1;
    }

    int shmId = shmget(shmKey, size, 0);
    if(shmId == -1) {
        perror("createSharedMemory: shmget");
        return -1;
    }

    return shmId;
}

char* attachSharedMemory(int shmId) {
    char *shm = shmat(shmId, NULL, 0);
    if(shm == (char*)(-1)) {
        perror("attachSharedMemory: shmat");
        return NULL;
    }

    return shm;
}

int detachSharedMemory(char* sharedMemory) {
    int result = shmdt(sharedMemory);

    if(result == -1) {
        perror("detachSharedMemory: shmdt");
    }

    return result;
}

int destroySharedMemory(int shmId) {
    int result = shmctl(shmId, IPC_RMID, NULL);

    if(result == -1) {
        perror("destroySharedMemory: shmctl");
    }

    return result;
}

bool waitingRoomPush(char* waitingRoom, int waitingRoomSize, char value) {
    int waitingClients = strlen(waitingRoom);
    if (waitingClients + 1 == waitingRoomSize) {
        return false;
    }

    waitingRoom[waitingClients] = value;
    waitingRoom[waitingClients + 1] = '\0';
    return true;
}

char waitingRoomPop(char* waitingRoom) {
    int waitingClients = strlen(waitingRoom);
    if (waitingClients == 0) {
        return '\0';
    }

    char value = waitingRoom[0];
    memcpy(waitingRoom, waitingRoom + 1, waitingClients + 1);
    return value;
}