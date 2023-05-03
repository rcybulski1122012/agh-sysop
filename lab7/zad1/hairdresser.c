#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "sem.h"
#include "shm.h"

bool isWaitingRoomEmpty(char* waitingRoom) {
    return strlen(waitingRoom) == 0;
}

int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    int waitingRoomSize = atoi(argv[1]);
    char* pathname = getenv("HOME");
    int shmId = getSharedMemoryId(pathname, waitingRoomSize);
    char* waitingRoom = attachSharedMemory(shmId);
    int hairdressersSemId = getSemaphoreId(pathname, HAIRDRESSERS_SEM_ID);
    int chairsSemId = getSemaphoreId(pathname, CHAIRS_SEM_ID);
    int blockOperationsSemId = getSemaphoreId(pathname, BLOCK_OPERATIONS_SEM_ID);
    bool shouldEndWork = true;

    while(true) {
        // the hairdresser is sleeping, a client has to wake them up
        // that's why it's decrement instead increment, which can be misleading
        decrementSemaphoreValue(hairdressersSemId);

        // blocks waiting room operations
        decrementSemaphoreValue(blockOperationsSemId);
        char hairstyle = waitingRoomPop(waitingRoom);
        incrementSemaphoreValue(blockOperationsSemId);

        printf("Hair dresser %d is serving a client. It will take %d seconds\n", pid, hairstyle);
        fflush(NULL);

        sleep(hairstyle);

        printf("Hair dresser %d has finished serving a client\n", pid);
        fflush(NULL);

        // increases number of available chairs
        incrementSemaphoreValue(chairsSemId);

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

    detachSharedMemory(waitingRoom);
    return 0;
}