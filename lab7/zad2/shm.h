#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include <unistd.h>
#include <sys/types.h>

int createSharedMemory(char* shmName, int size);
char* attachSharedMemory(char* shmName, int size);
int detachSharedMemory(char* sharedMemory, int size);
int destroySharedMemory(char* shmName);

bool waitingRoomPush(char* waitingRoom, int waitingRoomSize, char value);
char waitingRoomPop(char* waitingRoom);