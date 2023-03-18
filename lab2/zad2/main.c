#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef MY_BLOCK_SIZE
#define MY_BLOCK_SIZE 1024
#endif


void closeFiles(FILE *inputFile, FILE *outputFile)
{
    fclose(inputFile);
    fclose(outputFile);
}

void reverseSting(char* toReverse) {
    size_t len = strlen(toReverse);

    for (int i=0; i<len/2; i++){
        char tmp = toReverse[i];
        toReverse[i] = toReverse[len - i - 1];
        toReverse[len - i - 1] = tmp;
    }
}


void reverse(char *inputFilename, char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL)
    {
        puts("Failed to open the input file!");
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL)
    {
        puts("Failed to open the output file!");
        return;
    }

    char block[MY_BLOCK_SIZE];
    fseek(inputFile, 0, SEEK_END);
    size_t toRead = ftell(inputFile);
    size_t read;
    fseek(inputFile, -MY_BLOCK_SIZE, SEEK_END);

    while (toRead > 0) {
        if (toRead > MY_BLOCK_SIZE)
            read = fread(block, sizeof(char), MY_BLOCK_SIZE, inputFile);
        else
            read = fread(block, sizeof(char), toRead, inputFile);

        block[read] = 0;
        toRead -= read;
        reverseSting(block);

        fwrite(block, sizeof(char), read, outputFile);
        if (toRead > MY_BLOCK_SIZE)
            fseek(inputFile, -2 * MY_BLOCK_SIZE, SEEK_CUR);
        else
            fseek(inputFile, 0, SEEK_SET);
    }

    if (ferror(inputFile)) {
        puts("Failed to read the file!");
    }

    closeFiles(inputFile, outputFile);
}



int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        puts("Invalid number of arguments!");
        return -1;
    }
    char *inputFilename = argv[1];
    char *outputFilename = argv[2];

    struct timespec startTimespec, endTimespec;
    long execTime;

    clock_gettime(CLOCK_REALTIME, &startTimespec);
    reverse(inputFilename, outputFilename);
    clock_gettime(CLOCK_REALTIME, &endTimespec);

    execTime = endTimespec.tv_nsec - startTimespec.tv_nsec;
    printf("Czas wykonania: %lds\n", execTime);

    return 0;
}
