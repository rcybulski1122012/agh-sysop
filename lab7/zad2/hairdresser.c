#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sem.h"
#include "shm.h"
#include "common.h"


bool isWaitingRoomEmpty(char* waitingRoom) {
    return strlen(waitingRoom) == 0;
}


int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    int waitingRoomSize = atoi(argv[1]);
    char* waitingRoom = attachSharedMemory(WAITING_ROOM_SHM_NAME, waitingRoomSize);
    sem_t* hairdresserSem = getSemaphore(HAIRDRESSERS_SEM_NAME);
    sem_t* chairsSem = getSemaphore(CHAIRS_SEM_NAME);
    sem_t* blockOperationsSem = getSemaphore(BLOCK_OPERATIONS_SEM_NAME);
    bool shouldEndWork = true;

    while(true) {
        // the hairdresser is sleeping, a client has to wake them up
        // that's why it's decrement instead increment, which can be misleading
        sem_wait(hairdresserSem);

        // blocks waiting room operations
        sem_wait(blockOperationsSem);
        char hairstyle = waitingRoomPop(waitingRoom);
        sem_post(blockOperationsSem);

        printf("Hair dresser %d is serving a client. It will take %d seconds\n", pid, hairstyle);
        fflush(NULL);

        sleep(hairstyle);

        printf("Hair dresser %d has finished serving a client\n", pid);
        fflush(NULL);

        // increases number of available chairs
        sem_post(chairsSem);

        // checking if there is any waiting client
        if(isWaitingRoomEmpty(waitingRoom)) {
            for(int i=0; i<5; i++) {
                sleep(1);
                if(!isWaitingRoomEmpty(waitingRoom)) {
                    shouldEndWork = false;
                    break;
                }
            }
            if (shouldEndWork) {
                break;
            }
            else {
                shouldEndWork = true;
            }
        }
    }

    printf("Hair dresser %d has finished work for today due to lack of waiting clients\n", pid);

    sem_close(hairdresserSem);
    sem_close(chairsSem);
    sem_close(blockOperationsSem);
    detachSharedMemory(waitingRoom, waitingRoomSize);
    return 0;
}