#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void checkPending() {
    sigset_t waitingMask;
    sigpending(&waitingMask);
    if(sigismember(&waitingMask, SIGUSR1)) {
        printf("Signal %d is pending (%d)\n", SIGUSR1, getpid());
    } else {
        printf("Signal %d is not pending (%d)\n", SIGUSR1, getpid());
    }
}

int main(int argc, char* argv[]) {
    if(argc != 1) {
        puts("Invalid number of arguments");
        return 1;
    }

    char* action = argv[0];

    if (strcmp(action, "pending") == 0 ) {
        checkPending();
    } else {
        raise(SIGUSR1);
    }

    return 0;
}