#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define PRECISION 17

char buf0[BUF_SIZE] = "";
char buf1[BUF_SIZE] = "";
char buf2[BUF_SIZE] = "";

int countOccurrences(const char *str, char c) {
    int count = 0;
    const char *ptr = str;

    while ((ptr = strchr(ptr, c)) != NULL) {
        count++;
        ptr++;
    }

    return count;
}

double splitAndSum(char* str, char* separator) {
    double result = 0.0;
    char* token;
    char* end;

    token = strtok(str, separator);


    while (token != NULL) {
        double value = strtod(token, &end);
        if (token != end) {
            result += value;
        }
        token = strtok(NULL, separator);
    }

    return result;
}

double readAndSum(char* path, int numberOfLines) {
    int file = open(path, O_RDONLY);

    if (file == -1) {
        printf("Failed to open the file '%s'\n", path);
        return 0.0;
    }

    int newlineCharsRead = 0;
    char* fileContent = calloc(numberOfLines * BUF_SIZE + 1, sizeof(char));
    size_t bytesReadTotal = 0;
    size_t bytesRead = 0;

    while (newlineCharsRead < numberOfLines) {
        bytesRead = read(file, buf0, BUF_SIZE);
        bytesReadTotal += bytesRead;
        buf0[bytesRead] = '\0';

        newlineCharsRead += countOccurrences(buf0, '\n');
        strncat(fileContent, buf0, BUF_SIZE);
    }

    close(file);

    double result = splitAndSum(fileContent, "\n");
    free(fileContent);

    return result;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        puts("Invalid number of arguments!");
        return 1;
    }

    double rectWidth = strtod(argv[1], NULL);
    int processesCount = atoi(argv[2]);

    if(rectWidth <= 0 || processesCount <= 0) {
        puts("Both arguments must be positive!");
        return 2;
    }

    double processStep = 1.0/processesCount;


    struct timespec startTimespec, endTimespec;
    long execTime;
    clock_gettime(CLOCK_REALTIME, &startTimespec);

    if (mkfifo("./fifo", 0666) == -1) {
        puts("Failed to make FIFO!");
        return 3;
    }

    for(int i=0; i< processesCount; i++) {
        pid_t pid = fork();

        if(pid == 0) {
            snprintf(buf0, BUF_SIZE, "%g", processStep*i);
            snprintf(buf1, BUF_SIZE, "%g", processStep*(i+1));
            snprintf(buf2, BUF_SIZE,"%g", rectWidth);
            execl("./integral", buf0, buf1, buf2, NULL);
        }
    }


    double result = readAndSum("./fifo", processesCount);
    remove("./fifo");

    printf("Szerokość prostokąta: %g, liczba procesów: %d\n", rectWidth, processesCount);
    printf("Wynik: %g\n", result);

    clock_gettime(CLOCK_REALTIME, &endTimespec);
    execTime = endTimespec.tv_sec - startTimespec.tv_sec;
    printf("Czas wykonania: %lds\n\n", execTime);

    return 0;
}