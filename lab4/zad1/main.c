#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

enum Action{Ignore, Handler, Mask, Pending};

char* validArgs[] = {"ignore", "handler", "mask", "pending"};

bool isArgValid(char* arg) {
    for (int i=0; i<4; i++) {
        if (strcmp(validArgs[i], arg) == 0){
            return true;
        }
    }

    return false;
}

void handler(int signo) {
    printf("Signal %d was received (%d)\n", signo, getpid());
}

void ignore() {
    signal(SIGUSR1, SIG_IGN);
    raise(SIGUSR1);
}

void handle() {
    signal(SIGUSR1, handler);
    raise(SIGUSR1);
}

void mask() {
    signal(SIGUSR1, handler);
    sigset_t blocked;
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGUSR1);
    sigprocmask(SIG_BLOCK, &blocked, NULL);
    raise(SIGUSR1);
}

void checkPending() {
    sigset_t waitingMask;
    sigpending(&waitingMask);
    if(sigismember(&waitingMask, SIGUSR1)) {
        printf("Signal %d is pending (%d)\n", SIGUSR1, getpid());
    } else {
        printf("Signal %d is not pending (%d)\n", SIGUSR1, getpid());
    }
}

enum Action argToAction(char* arg) {
    if (strcmp(arg, "ignore") == 0) {
        return Ignore;
    }
    else if (strcmp(arg, "handler") == 0) {
        return Handler;
    }
    else if (strcmp(arg, "mask") == 0) {
        return Mask;
    }
    else {
        return Pending;
    }
}

void handleArg(enum Action action) {
    switch (action) {
        case Ignore:
            ignore();
            break;
        case Handler:
            handle();
            break;
        case Mask:
        case Pending:
            mask();
            break;
        default:
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
        return 1;
    }

    char* arg = argv[1];

    if (!isArgValid(arg)) {
        puts("Invalid argument");
        return 2;
    }

    enum Action action = argToAction(arg);

    handleArg(action);

    if (action == Pending) {
        checkPending();
    }

    fflush(NULL);

    #ifdef USE_EXEC
        if( execl("./test_exec", arg, NULL) == -1 ) {
            puts("Failed to execute the ./test_exec file");
            return 3;
        };
    #else
    pid_t pid = fork();

    if (pid == 0) {
        if (action == Pending) {
            checkPending();
        } else {
            raise(SIGUSR1);
        }
    } else {
        wait(NULL);
    }
    #endif

    fflush(NULL);

    return 0;
}