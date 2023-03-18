#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

#define MAX_PATH_SIZE 1024

char path[MAX_PATH_SIZE];
long long totalSize = 0;

int printFileSize(const char *path, const struct stat* sb, int tflag) {
    if(!S_ISDIR(sb->st_mode)) {
        printf("%s: %zuB\n", path, sb->st_size);
        totalSize += sb->st_size;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments!");
        return -1;
    }

    char* dirPath = argv[1];

    int result = ftw(dirPath, &printFileSize, 1);

    if(result == -1) {
        puts("An error occurred while traversing the file tree!");
    }

    printf("-----------------------------\nSumaryczny rozmiar plików: %lldB\n", totalSize);

    return 0;
}