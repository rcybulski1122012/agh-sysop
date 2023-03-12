#include <time.h>
#include <sys/times.h>
#include "dll_loader.h"
#define MAX_USER_INPUT_LENGTH 2048
#define MAX_ARG_LENGTH 2048

typedef enum {
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
    QUIT,
} Command;

char input[MAX_USER_INPUT_LENGTH];
char strArg[MAX_ARG_LENGTH];
Command commandType;
size_t sizeArg;
BlocksTable* table;
bool isRunning = true;
bool isInitialized = false;


bool startsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


bool parseInput() {
    sizeArg = 0;
    strArg[0] = '\0';
    int matched = 0;

    if (startsWith("init ", input)) {
        matched = sscanf(input, "init %zu", &sizeArg);
        if (matched != 1) {
            puts("Invalid input!");
            return false;
        }
        commandType = INIT;
        return true;
    }
    else if (startsWith("count ", input)) {
        matched = sscanf(input, "count %s", strArg);
        if (matched != 1) {
            puts("Invalid input!");
            return false;
        }
        commandType = COUNT;
        return true;
    }
    else if(startsWith("show ", input)) {
        matched = sscanf(input, "show %zu", &sizeArg);
        if (matched != 1) {
            puts("Invalid input!");
            return false;
        }
        commandType = SHOW;
        return true;
    }
    else if(startsWith("delete ", input)) {
        matched = sscanf(input, "delete %zu", &sizeArg);
        if (matched != 1) {
            puts("Invalid input!");
            return false;
        }
        commandType = DELETE;
        return true;
    }
    else if(startsWith("destroy", input)) {
        commandType = DESTROY;
        return true;
    }
    else if(startsWith("quit", input)) {
        commandType = QUIT;
        return true;
    }

    puts("Invalid input!");
    return false;
}

void destroyTable() {
    BlocksTable_free(table);
    free(table->removed);
    free(table->blocks);
    free(table);
}

void handleCommand() {
    if ((commandType != INIT && !isInitialized)) {
        if (commandType != QUIT){
            puts("The blocks table must be initialized beforehand!");
            return;
        }
    }

    switch (commandType) {
        case INIT:
            if (isInitialized) {
                puts("The blocks table can only be initialized once!");
                return;
            }

            if (sizeArg == 0) {
                puts("Cannot initialize a block table with size = 0!");
                return;
            }

            puts("init:");
            *table = BlocksTable_create(sizeArg);
            isInitialized = true;
            break;
        case COUNT:
            puts("count:");
            BlocksTable_countWords(table, strArg);
            break;
        case SHOW:
            puts("show:");
            char* result = BlocksTable_get(table, sizeArg);
            puts(result);
            break;
        case DELETE:
            puts("delete:");
            BlocksTable_remove(table, sizeArg);
            break;
        case DESTROY:
            puts("destroy");
            destroyTable();
            isInitialized = false;
            break;
        case QUIT:
            isRunning = false;
            if (isInitialized) destroyTable();
            break;
        default:
            puts("Failed to handle the command!");
    }
}


void printExecTimes(
    struct tms startTms,
    struct tms endTms,
    struct timespec startTimespec,
    struct timespec endTimespec
) {

    printf("Real execution time: %ld ns\n", endTimespec.tv_nsec - startTimespec.tv_nsec);
    printf("User execution time: %ld\n", endTms.tms_utime - startTms.tms_utime);
    printf("System execution time: %ld\n", endTms.tms_stime - startTms.tms_stime);
}


int main() {
    loadDLL("libwc.so");
    table = malloc(sizeof(BlocksTable));
    struct tms startTms, endTms;
    struct timespec startTimespec, endTimespec;

    while (isRunning) {
        printf(">>> ");
        if(fgets(input, MAX_USER_INPUT_LENGTH, stdin) == NULL) {
            puts("Failed to get user's input!");
            return -1;
        }
        input[strcspn(input, "\r\n")] = 0;
        if(!parseInput()) {
            continue;
        }

        clock_gettime(CLOCK_REALTIME, &startTimespec);
        times(&startTms);

        handleCommand();

        clock_gettime(CLOCK_REALTIME, &endTimespec);
        times(&endTms);

        printExecTimes(startTms, endTms, startTimespec, endTimespec);
    }

    return 0;
}