#include "shm.h"


int createSharedMemory(char* shmName, int size) {
    int fd = shm_open(shmName, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("createSharedMemory: shm_open");
        return -1;
    }

    if(ftruncate(fd, size) == -1) {
        perror("createSharedMemory: ftruncate");
        return -1;
    }

    return fd;
}

char* attachSharedMemory(char* shmName, int size) {
    int fd = shm_open(shmName, O_RDWR, 0666);
    if(fd == -1) {
        perror("attachSharedMemory: shm_open");
        return NULL;
    }

    if(ftruncate(fd, size) == -1) {
        perror("attachSharedMemory: ftruncate");
    }

    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

int detachSharedMemory(char* sharedMemory, int size) {
    return munmap(sharedMemory, size);
}

int destroySharedMemory(char* shmName) {
    return shm_unlink(shmName);
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