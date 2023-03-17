#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef USE_SYS_FUNC
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void closeFiles(int inputFile, int outputFile)
{
    close(inputFile);
    close(outputFile);
}

void copyAndReplace(char toReplace, char replaceWith, char *inputFilename, char *outputFilename)
{
    int inputFile = open(inputFilename, O_RDONLY);
    if (inputFile == -1)
    {
        puts("Failed to open the input file!");
        return;
    }

    int outputFile = open(outputFilename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (outputFile == -1)
    {
        puts("Failed to open the output file!");
        return;
    }

    char chr;
    while (read(inputFile, &chr, sizeof(char)) == 1)
    {
        if (chr == toReplace)
        {
            chr = replaceWith;
        }

        if (write(outputFile, &chr, sizeof(char)) != 1)
        {
            puts("Failed to write file content!");
            closeFiles(inputFile, outputFile);
            return;
        }
    }

    closeFiles(inputFile, outputFile);
    return;
}

#else
void closeFiles(FILE *inputFile, FILE *outputFile)
{
    fclose(inputFile);
    fclose(outputFile);
}

void copyAndReplace(char toReplace, char replaceWith, char *inputFilename, char *outputFilename)
{
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

    char chr;
    while (fread(&chr, sizeof(char), 1, inputFile) == 1)
    {
        if (chr == toReplace)
        {
            chr = replaceWith;
        }

        if (fwrite(&chr, sizeof(char), 1, outputFile) != 1)
        {
            puts("Failed to write file content!");
            closeFiles(inputFile, outputFile);
            return;
        }
    }

    if (ferror(inputFile) != 0)
    {
        puts("Failed to read file content!");
        closeFiles(inputFile, outputFile);
        return;
    }

    closeFiles(inputFile, outputFile);
}
#endif

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        puts("Invalid number of arguments!");
        return -1;
    }

    char toReplace = argv[1][0];
    char replaceWith = argv[2][0];
    char *inputFilename = argv[3];
    char *outputFilename = argv[4];

    struct timespec startTimespec, endTimespec;
    long execTime;

    clock_gettime(CLOCK_REALTIME, &startTimespec);
    copyAndReplace(toReplace, replaceWith, inputFilename, outputFilename);
    clock_gettime(CLOCK_REALTIME, &endTimespec);

    execTime = endTimespec.tv_nsec - startTimespec.tv_nsec;
    printf("Czas wykonania: %lds\n", execTime);
    return 0;
}