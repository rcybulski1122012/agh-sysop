#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 1024

char path[MAX_PATH_SIZE];

int main() {
    long long totalSize = 0;
    DIR* dir;

    if ((dir = opendir(".")) == NULL) {
        puts("Failed to open the current directory!");
        return -1;
    }

    struct dirent* entry;
    struct stat* buf = malloc(sizeof(struct stat));

    while((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(path, MAX_PATH_SIZE, "./%s", entry->d_name);
        stat(path, buf);

        if(S_ISDIR(buf->st_mode)) {
            continue;
        }

        totalSize += buf->st_size;
        printf("%s: %zuB\n",entry->d_name, buf->st_size);
    }

    printf("-----------------------------\nSumaryczny rozmiar plików: %lldB\n", totalSize);

    if (closedir(dir) == -1){
        puts("Failed to close the directory!");
        return -1;
    }

    return 0;
}