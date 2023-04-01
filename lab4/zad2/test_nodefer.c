#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int calls = 0;
int depth = 0;


void handlerSA_NODEFER(int sig, siginfo_t* info, void* ucontext) {
    printf("depth: %d\n", depth);

    calls++;
    depth++;
    if (calls < 10) {
        raise(SIGUSR1);
    }
    depth--;

    printf("depth: %d\n", depth);
}


int main() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handlerSA_NODEFER;

    puts("With SA_NODEFER");
    action.sa_flags = SA_NODEFER;
    sigaction(SIGUSR1, &action, NULL);
    raise(SIGUSR1);

    puts("\n\n");

    puts("Without SA_NODEFER");
    calls = 0;
    depth = 0;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);
    raise(SIGUSR1);

    return 0;
}