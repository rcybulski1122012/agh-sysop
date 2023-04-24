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

MsgBuf msgBuf;
char msgContentBuf[MAX_MSG_LENGTH] = "";

bool isRunning = true;


void resetMsgBuf() {
    msgBuf.clientId = -1;
    msgBuf.msgType = -1;
    msgBuf.msgContent[0] = '\0';
    msgBuf.queueKey = -1;
}


void stopServer() {
    isRunning = false;

    resetMsgBuf();
    msgBuf.msgType = STOP;
    for(int i=0; i<MAX_CLIENTS_ID; i++) {
        if(clientsQueues[i] != -1) {
            int queueId = msgget(clientsQueues[i], 0);
            msgsnd(queueId, &msgBuf, MSG_BUF_SIZE, 0);
        }
    }

    msgctl(msqId, IPC_RMID, NULL);
}


void initServer() {
    for(int i=0; i< MAX_CLIENTS_ID; i++) {
        clientsQueues[i] = -1;
    }

    signal(SIGINT, stopServer);
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


void logMsg(MsgBuf msgBuf) {
    logfd = fopen("./logs", "a");
    time(&now);
    timeinfo = localtime(&now);
    snprintf(
        logBuffer, LOG_BUFF_SIZE,
        "%s    Message Type: %lu\n    Client Id: %d\n    Receiver Id: %d\n    Message Content: %s\n\n",
         asctime(timeinfo), msgBuf.msgType, msgBuf.clientId, msgBuf.receiverId, msgBuf.msgContent
    );
    puts(logBuffer);
    fwrite(logBuffer, sizeof(char), strlen(logBuffer), logfd);
    fclose(logfd);
}


void handle_INIT(key_t queueKey) {
    resetMsgBuf();
    int clientId = getClientId();

    if(clientId != -1) {
        clientsQueues[clientId] = queueKey;
    }

    msgBuf.clientId = clientId;
    msgBuf.msgType = INIT;
    int clientQueueId = msgget(queueKey, 0);
    msgsnd(clientQueueId, &msgBuf, MSG_BUF_SIZE, 0);
}

void handle_STOP(int clientId) {
    clientsQueues[clientId] = -1;
}


void handle_LIST(int clientId, key_t queueKey) {
    resetMsgBuf();
    char buf[MAX_CLIENTS_ID * 4] = "";
    sprintf(msgBuf.msgContent, "Active users:");
    for (int i=0; i<MAX_CLIENTS_ID; i++) {
        if(clientsQueues[i] != -1 && i != clientId) {
            snprintf(buf, 32, "%d, ", i);
            strcat(msgBuf.msgContent, buf);
        }
    }
    msgBuf.msgType = LIST;
    int clientQueueId = msgget(queueKey, 0);
    msgsnd(clientQueueId, &msgBuf, MSG_BUF_SIZE, 0);
}

void handle_TO_ALL(int senderId, char* message) {
    resetMsgBuf();
    time(&now);
    timeinfo = localtime(&now);
    msgBuf.sentAt = *timeinfo;
    msgBuf.msgType = PRINT;
    msgBuf.clientId = senderId;
    snprintf(msgBuf.msgContent, MAX_MSG_LENGTH, "%s", message);
    for (int i=0; i<MAX_CLIENTS_ID; i++) {
        if(i != senderId && clientsQueues[i] != -1) {
            key_t receiverKey = clientsQueues[i];
            int receiverQueueId = msgget(receiverKey, 0);
            msgsnd(receiverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
        }
    }
}


void handle_TO_ONE(int senderId, int receiverId, char* message) {
    resetMsgBuf();
    if(receiverId >= MAX_CLIENTS_ID || clientsQueues[receiverId] == -1) {
        return;
    }

    key_t receiverKey = clientsQueues[receiverId];
    int receiverQueueId = msgget(receiverKey, 0);

    time(&now);
    timeinfo = localtime(&now);
    msgBuf.sentAt = *timeinfo;
    msgBuf.msgType = PRINT;
    msgBuf.clientId = senderId;
    snprintf(msgBuf.msgContent, MAX_MSG_LENGTH, "%s", message);
    msgsnd(receiverQueueId, &msgBuf, MSG_BUF_SIZE, 0);
}



int main() {
    char* homePath = getenv("HOME");
    if (homePath == NULL) {
        perror("getenv");
    }

    key_t queueKey = ftok(getenv("HOME"), SERVER_ID);
    msqId = msgget(queueKey, 0666 | IPC_CREAT);

    initServer();

    resetMsgBuf();
    while(isRunning) {
        msgrcv(msqId, &msgBuf, MSG_BUF_SIZE, ALL_MESSAGES, 0);
        strcpy(msgContentBuf, msgBuf.msgContent);
        logMsg(msgBuf);
        switch (msgBuf.msgType) {
            case INIT:
                handle_INIT(msgBuf.queueKey);
                break;
            case STOP:
                handle_STOP(msgBuf.clientId);
                break;
            case LIST:
                handle_LIST(msgBuf.clientId, clientsQueues[msgBuf.clientId]);
                break;
            case TO_ALL:
                handle_TO_ALL(msgBuf.clientId, msgContentBuf);
                break;
            case TO_ONE:
                handle_TO_ONE(msgBuf.clientId, msgBuf.receiverId, msgContentBuf);
                break;
            default:
                continue;
        }
    }

    return 0;
}