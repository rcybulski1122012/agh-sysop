#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define INPUT_SIZE 255

char fileBuf[INPUT_SIZE];

void readNCharsFromFile(char* fileName, int n, char* buf) {
    buf[0] = '\0';
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Failed to open the file");
        return;
    }
    int read = fread(fileBuf, sizeof(char), n, file);

    if (ferror(file) != 0) {
        perror("Failed to read the file");
        fclose(file);
        return;
    }

    if (read != n) {
        buf[0] = '\0';
        return;
    } else {
        buf[read] = '\0';
    }

    fclose(file);
}


void scanCatalogue(char* pathToScan, char* startWith, size_t startWithLength) {
    DIR* dir;

    if ((dir = opendir(pathToScan)) == NULL) {
        perror("Failed to open the directory!");
        return;
    }

    struct dirent* entry;
    struct stat buf;
    char currPath[PATH_MAX];

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(currPath, PATH_MAX, "%s/%s", pathToScan, entry->d_name);
        stat(currPath, &buf);

        if(S_ISDIR(buf.st_mode)) {
            pid_t pid = fork();
            if (pid == 0) {
                scanCatalogue(currPath, startWith, startWithLength);
                break;
            }
        }
        else {
            if (access(currPath, R_OK) == -1) {
                continue;
            }

            readNCharsFromFile(currPath, startWithLength, fileBuf);

            if (strcmp(fileBuf, startWith) == 0) {
                printf("path: %s,  id: %d\n", currPath, getpid());
            }
        }
    }

    if (closedir(dir) == -1){
        perror("Failed to close the directory!");
    }
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        puts("Invalid number of arguments");
        return -1;
    }

    char* startPath = argv[1];
    char* startWith = argv[2];
    char realPath[PATH_MAX];
    size_t length = strlen(startWith);
    if (realpath(startPath, realPath) == NULL) {
        perror("Invalid path!");
    }

    if (length > 255) {
        puts("The maximum length of the second arguments is 255");
        return -1;
    }

    scanCatalogue(realPath, startWith, length);
    while (wait(NULL) > 0) {
        continue;
    }
    return 0;
}