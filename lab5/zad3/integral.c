#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PRECISION 17

char buf[BUF_SIZE];



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

    double start = strtod(argv[0], NULL);
    double stop = strtod(argv[1], NULL);
    double step = strtod(argv[2], NULL);


    if (start >= stop || step <= 0 ) {
        puts("Invalid values of arguments!");
        return 2;
    }

    double result = computeValue(start, stop, step);
    snprintf(buf, BUF_SIZE, "%.*g\n", PRECISION, result);

    int file = open("./fifo", O_WRONLY);
    if (file == -1) {
        puts("Failed to open the file!");
        return 3;
    }

    int toWrite = strlen(buf);
    int written = write(file, buf, toWrite);

    if(toWrite > written) {
        puts("Failed to write to the file!");
    }

    close(file);
    return 0;
}