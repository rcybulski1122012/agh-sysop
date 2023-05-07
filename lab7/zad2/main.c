#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "sem.h"
#include "shm.h"

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

    createSharedMemory(WAITING_ROOM_SHM_NAME, waitingRoomSize);
    sem_t* hairdressersSem = createSemaphore(HAIRDRESSERS_SEM_NAME, 0);
    sem_t* chairsSem = createSemaphore(CHAIRS_SEM_NAME, 0);
    sem_t* blockOperationsSem = createSemaphore(BLOCK_OPERATIONS_SEM_NAME, 1);
    snprintf(argBuf, BUFF_SIZE, "%d", waitingRoomSize);

    if(hairdressersSem == NULL || chairsSem == NULL || blockOperationsSem == NULL) {
        return 3;
    }

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


    sem_close(hairdressersSem);
    sem_close(chairsSem);
    sem_close(blockOperationsSem);

    sem_unlink(HAIRDRESSERS_SEM_NAME);
    sem_unlink(CHAIRS_SEM_NAME);
    sem_unlink(BLOCK_OPERATIONS_SEM_NAME);
    destroySharedMemory(WAITING_ROOM_SHM_NAME);
    sleep(1);
    return 0;
}