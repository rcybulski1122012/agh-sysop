#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#define BUF_SIZE 1024

double func(double x) {
    return 4 / (x*x + 1);
}

double computeValue(double start, double stop, double step) {
    double result = 0;

    while (start < stop) {
        result += func(start + step/2) * step;
        start += step;
    }

    return result;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        puts("Invalid number of arguments!");
        return 1;
    }

    char* buf = calloc(BUF_SIZE, sizeof(char));
    double rectWidth = strtod(argv[1], NULL);
    int processesCount = atoi(argv[2]);

    if(rectWidth <= 0 || processesCount <= 0) {
        puts("Both arguments must be positive!");
        return 2;
    }

    double processStep = 1.0/processesCount;
    int* readPipes = calloc(processesCount, sizeof(int));

    struct timespec startTimespec, endTimespec;
    long execTime;
    clock_gettime(CLOCK_REALTIME, &startTimespec);

    for(int i=0; i<processesCount; i++) {
        int fd[2];
        if (pipe(fd) == -1) {
            puts("Failed to open a pipe!");
            return 3;
        }
        pid_t pid = fork();

        if (pid == 0) {
            close(fd[1]);
            readPipes[i] = fd[0];
            continue;
        } else {
            close(fd[0]);
            double value = computeValue(processStep*i, processStep*(i+1), rectWidth);
            snprintf(buf, BUF_SIZE, "%g", value);
            if(write(fd[1], buf, strlen(buf)) == -1) {
                puts("Failed to write to a pipe file descriptor!");
                return 4;
            }
            return 0;
        }
    }

    while (wait(NULL) > 0) {
        continue;
    }

    double result = 0;

    for (int i=0; i<processesCount; i++) {
        if (read(readPipes[i], buf, BUF_SIZE) == 0) {
            puts("Failed to read from a pipe file description!");
            return 5;
        }
        double value = strtod(buf, NULL);
        result += value;
        close(readPipes[i]);
    }

    printf("Szerokość prostokąta: %g, liczba procesów: %d\n", rectWidth, processesCount);
    printf("Wynik: %g\n", result);

    clock_gettime(CLOCK_REALTIME, &endTimespec);
    execTime = endTimespec.tv_sec - startTimespec.tv_sec;
    printf("Czas wykonania: %lds\n\n", execTime);

    free(readPipes);
    free(buf);

    return 0;
}