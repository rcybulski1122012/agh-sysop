#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
    }

    char* path = argv[1];

    printf("%s ", path);

    fflush(stdout);
    execl("/bin/ls", "ls", path, NULL);

    return 0;
}