#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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
    char* pathname = getenv("HOME");
    int shmId = getSharedMemoryId(pathname, waitingRoomSize);
    char* waitingRoom = attachSharedMemory(shmId);
    int hairdressersSemId = getSemaphoreId(pathname, HAIRDRESSERS_SEM_ID);
    int chairsSemId = getSemaphoreId(pathname, CHAIRS_SEM_ID);
    int blockOperationsSemId = getSemaphoreId(pathname, BLOCK_OPERATIONS_SEM_ID);

    if(isWaitingRoomFull(waitingRoom, waitingRoomSize)) {
        printf("Client %d left the hair salon because the waiting room was full\n", pid);
        fflush(NULL);
        return 1;
    }

    // blocks waiting room operations
    decrementSemaphoreValue(blockOperationsSemId);
    char hairstyle = getRandomHairstyle(pid);
    printf("Client %d came to the hair salon. (%d)\n", pid, hairstyle);
    fflush(NULL);
    waitingRoomPush(waitingRoom, waitingRoomSize, hairstyle);
    incrementSemaphoreValue(blockOperationsSemId);

    // the client is already in the waiting room, so we don't need to use IPC_NOWAIT
    // wakes the first sleeping hairdresser if any available
    incrementSemaphoreValue(hairdressersSemId);

    // takes first available chair, waits in the queue if there is no any
    decrementSemaphoreValue(chairsSemId);

    detachSharedMemory(waitingRoom);
    return 0;
}