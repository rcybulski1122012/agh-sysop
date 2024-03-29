#include <sys/ipc.h>
#include <time.h>
#define MAX_MSG_LENGTH 512
#define SERVER_ID 0
#define MAX_CLIENTS_ID 16
#define LOG_BUFF_SIZE 4096
#define COMMAND_BUFF_SIZE 1024


// values are also priorities
typedef enum MsgType {
    INIT = 1,
    TO_ALL = 2,
    TO_ONE = 3,
    LIST = 4,
    STOP = 5,
    PRINT = 6,
} MsgType;

int ALL_MESSAGES = -7;

typedef struct MsgBuf {
    long msgType;
    char msgContent[MAX_MSG_LENGTH];
    int clientId;
    int receiverId;
    key_t queueKey;
    struct tm sentAt;
} MsgBuf;


int MSG_BUF_SIZE = sizeof(MsgBuf);
