#include "common.h"
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

key_t clientsQueues[MAX_CLIENTS_ID];
int msqId;

FILE* logfd;
char logBuffer[LOG_BUFF_SIZE] = "";

time_t now;
struct tm *timeinfo;



void initServer() {
    for(int i=0; i< MAX_CLIENTS_ID; i++) {
        clientsQueues[i] = -1;
    }
}


int getClientId() {
    int id = 0;
    while(id < MAX_CLIENTS_ID) {
        if (clientsQueues[id] == -1) {
            return id;
        }
        id++;
    }

    return -1;
}


void logMsg(MsgBuf* msgBuf) {
    time(&now);
    timeinfo = localtime(&now);
    snprintf(
        logBuffer, LOG_BUFF_SIZE,
        "%s    Message Type: %lu\n    Client Id: %d\n    Message Content: %s\n\n",
         asctime(timeinfo), msgBuf->msgType, msgBuf->clientId, msgBuf->msgContent
    );
    puts(logBuffer);
    fwrite(logBuffer, sizeof(char), strlen(logBuffer), logfd);
}


void handle_INIT(key_t queueKey) {
    MsgBuf *msgBuf = malloc(sizeof(MsgBuf));
    int clientId = getClientId();

    if(clientId != -1) {
        clientsQueues[clientId] = queueKey;
    }

    msgBuf->clientId = clientId;
    msgBuf->msgType = INIT;
    int clientQueueId = msgget(queueKey, 0);
    msgsnd(clientQueueId, msgBuf, MSG_BUF_SIZE, 0);
    free(msgBuf);
}

void handle_STOP() {

}




int main() {
    char* homePath = getenv("HOME");
    if (homePath == NULL) {
        perror("getenv");
    }

    key_t queueKey = ftok(getenv("HOME"), SERVER_ID);
    msqId = msgget(queueKey, 0666 | IPC_CREAT);

    printf("queueKey=%d, queueId=%d\n", queueKey, msqId);

    logfd = fopen("./logs", "w");
    initServer();

    MsgBuf *msgBuf = malloc(sizeof(MsgBuf));
    while(true) {
        msgrcv(msqId, msgBuf, MSG_BUF_SIZE, ALL_MESSAGES, 0);
        logMsg(msgBuf);
        switch (msgBuf->msgType) {
            case INIT:
                handle_INIT(msgBuf->queueKey);
                break;
            default:
                continue;
        }
    }

    free(msgBuf);
    fclose(logfd);
    msgctl(msqId, IPC_RMID, NULL);

    return 0;
}