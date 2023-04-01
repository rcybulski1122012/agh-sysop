#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void handlerSA_RESETHAND(int sig, siginfo_t* info, void* ucontext) {
    puts("SA_RESETHAND flag test");
}


int main() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGUSR1);
    action.sa_sigaction = handlerSA_RESETHAND;
    action.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, &action, NULL);
    raise(SIGUSR1);
    raise(SIGUSR1);

    return 0;
}