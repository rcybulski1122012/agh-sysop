#include "common.h"
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int clientId;
int myQueueId;
key_t myKey;
int serverQueueId;
key_t serverQueueKey;
MsgType msgType;
MsgBuf msgBuf;

char commandBuf[COMMAND_BUFF_SIZE] = "";
int intArg;
char strArg[MAX_MSG_LENGTH] = "";

void resetMsgBut() {
    msgBuf.clientId = -1;
    msgBuf.msgType = -1;
    msgBuf.msgContent[0] = '\0';
    msgBuf.queueKey = -1;
}


void init() {
    resetMsgBut();
    myKey = ftok(getenv("HOME"), getpid() % MAX_CLIENTS_ID + 1);
    msgBuf.msgType = INIT;
    msgBuf.queueKey = myKey;

    msgsnd(serverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
    myQueueId = msgget(myKey, 0666 | IPC_CREAT);

    msgrcv(myQueueId, &msgBuf, MSG_BUF_SIZE, ALL_MESSAGES, 0);
    clientId = msgBuf.clientId;



    if(clientId == -1) {
        puts("Failed to connect to the server!");
        exit(0);
    }
    else {
        printf("Connected to the server. Your id: %d\n", clientId);
    }
}


bool startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


bool parseInput() {
    int matched = 0;
    intArg = -1;
    strArg[0] = '\0';

    if(startsWith("\n", commandBuf)) {
        return false;
    }
    else if(startsWith("STOP", commandBuf)) {
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
    resetMsgBut();
    msgBuf.clientId = clientId;
    msgBuf.msgType = STOP;
    msgsnd(serverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
    msgctl(myQueueId, IPC_CREAT, NULL);
    exit(0);
}


void handleLIST() {
    resetMsgBut();
    msgBuf.clientId = clientId;
    msgBuf.msgType = LIST;
    msgsnd(serverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
    msgrcv(myQueueId, &msgBuf, MSG_BUF_SIZE, LIST, 0);
    puts(msgBuf.msgContent);
}


void handle2ONE() {
    resetMsgBut();
    msgBuf.clientId = clientId;
    msgBuf.receiverId = intArg;
    msgBuf.msgType = TO_ONE;
    snprintf(msgBuf.msgContent, MAX_MSG_LENGTH, "%s", strArg);
    msgsnd(serverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
}


void handle2ALL() {
    resetMsgBut();
    msgBuf.clientId = clientId;
    msgBuf.msgType = TO_ALL;
    snprintf(msgBuf.msgContent, MAX_MSG_LENGTH, "%s", strArg);
    msgsnd(serverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
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

void listenPRINTandSTOP() {
    resetMsgBut();
    while(msgrcv(myQueueId, &msgBuf, MSG_BUF_SIZE, PRINT, IPC_NOWAIT) != -1) {
        printf("Sent at: %s From: %d\n Message:\n%s\n", asctime(&msgBuf.sentAt), msgBuf.clientId, msgBuf.msgContent);
    }

    if(msgrcv(myQueueId, &msgBuf, MSG_BUF_SIZE, STOP, IPC_NOWAIT) != -1) {
        puts("SERVER STOPPED");
        handleSTOP();
        exit(0);
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

    init();
//    atexit(handleSTOP);
    signal(SIGINT, handleSTOP);


    while(1) {
        printf(">>> ");
        if(fgets(commandBuf, COMMAND_BUFF_SIZE, stdin) == NULL) {
            puts("Failed to get user's input!");
            return -1;
        }
        listenPRINTandSTOP();

        if(!parseInput()) {
            continue;
        }

        handleCommand();
    }
}