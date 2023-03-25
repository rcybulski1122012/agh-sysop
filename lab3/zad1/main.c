#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
    }

    int toCreate = atoi(argv[1]);

    if (toCreate == 0) {
        puts("Invalid number of processes!");
    }

    pid_t pid;

    for (int i=0; i<toCreate; i++) {
        pid = fork();

        if (pid == 0) {
            break;
        }
    }

    if (pid == 0) {
        printf("Parent Process Id: %d, Process Id: %d\n", getppid(), getpid());
    }
    else {
        while (wait(NULL) > 0) {
            continue;
        };
        printf("Number of processes run: %d\n", toCreate);
    }

    return 0;
}