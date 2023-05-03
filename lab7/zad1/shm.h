#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"


int createSharedMemory(char* pathname, int size);
int getSharedMemoryId(char* pathname, int size);
char* attachSharedMemory(int shmId);
int detachSharedMemory(char* sharedMemory);
int destroySharedMemory(int shmId);

bool waitingRoomPush(char* waitingRoom, int waitingRoomSize, char value);
char waitingRoomPop(char* waitingRoom);