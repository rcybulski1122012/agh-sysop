#include <signal.h>
#include <stdio.h>

int calls = 0;

void handleSA_SIGNFO(int sig, siginfo_t* info, void* ucontext) {
    printf("Signal info:\n");
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumer: %lu\n", info->si_utime);
    printf("User id of sending process: %d\n", info->si_uid);
    printf("Signal number: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
}

int main() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handleSA_SIGNFO;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);
    raise(SIGUSR1);


    return 0;
}