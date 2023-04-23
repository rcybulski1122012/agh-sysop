#include "common.h"
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int clientId;
int myQueueId;
key_t myKey;
int serverQueueId;
key_t serverQueueKey;
MsgType msgType;

char commandBuf[COMMAND_BUFF_SIZE] = "";
int intArg;
char strArg[MAX_MSG_LENGTH] = "";


void init() {
    myKey = ftok(getenv("HOME"), getpid());
    MsgBuf *msgBuf = malloc(sizeof(MsgBuf));
    msgBuf->msgType = INIT;
    msgBuf->queueKey = myKey;

    msgsnd(serverQueueId, msgBuf, MSG_BUF_SIZE, 0);
    myQueueId = msgget(myKey, 0666 | IPC_CREAT);


    msgrcv(myQueueId, msgBuf, MSG_BUF_SIZE, ALL_MESSAGES, 0);
    clientId = msgBuf->clientId;

    if(clientId == -1) {
        puts("Failed to connect to the server!");
        exit(0);
    }
    free(msgBuf);
}


bool startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


bool parseInput() {
    int matched = 0;

    if(startsWith("STOP", commandBuf)) {
        msgType = STOP;
        return true;
    }
    else if(startsWith("LIST", commandBuf)) {
        msgType = LIST;
        return true;
    }
    else if(startsWith("2ALL", commandBuf)) {
        msgType = TO_ALL;
        matched = sscanf(commandBuf, "2ALL %s", strArg);
        if(matched != 1) {
            puts("Invalid input!");
            return false;
        }
        return true;
    }
    else if(startsWith("2ONE", commandBuf)) {
        msgType = TO_ONE;
        matched = sscanf(commandBuf, "2ONE %d %s", &intArg, strArg);
        if (matched != 2) {
            puts("Invalid input!");
            return false;
        }
        return true;
    }

    puts("Invalid input!");
    return false;
}


void handleSTOP() {

}


void handleLIST() {
    puts("LIST");
}


void handle2ONE() {
    puts("2ONE");
}


void handle2ALL() {
    puts("2ALL");
}


void handleCommand() {
    switch (msgType) {
        case STOP:
            handleSTOP();
            break;
        case LIST:
            handleLIST();
            break;
        case TO_ONE:
            handle2ONE();
            break;
        case TO_ALL:
            handle2ALL();
            break;
        default:
            break;
    }
}


int main() {
    char* homePath = getenv("HOME");
    if (homePath == NULL) {
        perror("getenv");
    }

    serverQueueKey = ftok(getenv("HOME"), SERVER_ID);
    serverQueueId = msgget(serverQueueKey, 0);

    if (serverQueueId == -1) {
        perror("msgget");
    }

    // replace with getpid
    init();
    signal(SIGINT, handleSTOP);

    while(1) {
        printf("[%d]>>> ", clientId);
        if(fgets(commandBuf, COMMAND_BUFF_SIZE, stdin) == NULL) {
            puts("Failed to get user's input!");
            return -1;
        }
        if(!parseInput()) {
            continue;
        }

        handleCommand();
    }
}