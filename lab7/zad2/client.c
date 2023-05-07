#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "sem.h"
#include "shm.h"

char getRandomHairstyle(pid_t pid) {
    return (char) (rand() % 5 + 1);
}

bool isWaitingRoomFull(char * waitingRoom, int waitingRoomSize) {
    return strlen(waitingRoom) + 1 == waitingRoomSize;
}


int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    srand(time(NULL));
    int waitingRoomSize = atoi(argv[1]);
    char* waitingRoom = attachSharedMemory(WAITING_ROOM_SHM_NAME, waitingRoomSize);
    sem_t* hairdresserSem = getSemaphore(HAIRDRESSERS_SEM_NAME);
    sem_t* chairsSem = getSemaphore(CHAIRS_SEM_NAME);
    sem_t* blockOperationsSem = getSemaphore(BLOCK_OPERATIONS_SEM_NAME);

    if(isWaitingRoomFull(waitingRoom, waitingRoomSize)) {
        printf("Client %d left the hair salon because the waiting room was full\n", pid);
        fflush(NULL);
        return 1;
    }

    // blocks waiting room operations
    sem_wait(blockOperationsSem);
    char hairstyle = getRandomHairstyle(pid);
    printf("Client %d came to the hair salon. (%d)\n", pid, hairstyle);
    fflush(NULL);
    waitingRoomPush(waitingRoom, waitingRoomSize, hairstyle);
    sem_post(blockOperationsSem);

    // the client is already in the waiting room, so we don't need to use IPC_NOWAIT
    // wakes the first sleeping hairdresser if any available
    sem_post(hairdresserSem);

    // takes first available chair, waits in the queue if there is no any
    sem_wait(chairsSem);

    sem_close(hairdresserSem);
    sem_close(chairsSem);
    sem_close(blockOperationsSem);
    detachSharedMemory(waitingRoom, waitingRoomSize);
    return 0;
}