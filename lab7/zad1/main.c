#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include "shm.h"
#include "sem.h"

#define BUFF_SIZE 32

bool areArgsValid(
    int hairdressersCount,
    int chairsCount,
    int waitingRoomSize,
    int clientsCount
) {
    return (
        hairdressersCount >= chairsCount
        && hairdressersCount > 0
        && chairsCount > 0
        && waitingRoomSize >= 0
        && clientsCount > 0
    );
}

char argBuf[BUFF_SIZE] = "";

int shmId;
int hairdressersSemId;
int chairsSemId;
int waitingRoomSemId;
int blockOperationsSemId;


int main(int argc, char* argv[]) {
    if (argc != 5) {
        puts("Invalid number of arguments");
        return 1;
    }

    int hairdressersCount = atoi(argv[1]);
    int chairsCount = atoi(argv[2]);
    int waitingRoomSize = atoi(argv[3]);
    int clientsCount = atoi(argv[4]);

    if (!areArgsValid(hairdressersCount, chairsCount, waitingRoomSize, clientsCount)) {
        puts("Invalid arguments");
        return 2;
    }

    char* pathname = getenv("HOME");
    shmId = createSharedMemory(pathname, waitingRoomSize);
    hairdressersSemId = createSemaphore(pathname, HAIRDRESSERS_SEM_ID, 0);
    chairsSemId = createSemaphore(pathname, CHAIRS_SEM_ID, 0);
    blockOperationsSemId = createSemaphore(pathname, BLOCK_OPERATIONS_SEM_ID, 1);
    snprintf(argBuf, BUFF_SIZE, "%d", waitingRoomSize);

    for(int i=0; i<hairdressersCount; i++) {
        if(fork() == 0) {
            execl("./hairdresser", "./hairdresser", argBuf, NULL);
        }
    }

    srand(time(NULL));
    for(int i=0; i<clientsCount; i++) {
        usleep(rand() % 1000000);    // max 1 second
        if(fork() == 0) {
            execl("./client", "./client", argBuf, NULL);
        }
    }


    while(wait(NULL) > 0) {
        continue;
    }

    destroySemaphore(hairdressersSemId);
    destroySemaphore(chairsSemId);
    destroySemaphore(blockOperationsSemId);
    destroySharedMemory(shmId);

    return 0;
}