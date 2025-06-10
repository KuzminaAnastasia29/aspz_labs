#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

void log_signal(int sig) {
    FILE *f = fopen("/tmp/signal_log.txt", "a");
    if (f) {
        time_t now = time(NULL);
        fprintf(f, "[%s] Received signal: %d from PID: %d UID: %d\n",
                ctime(&now), sig, getpid(), getuid());
        fclose(f);
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    setsid();

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    for (int i = 0; i < 1024; i++) close(i);
}

int main() {
    daemonize();

    signal(SIGINT, log_signal);
    signal(SIGTERM, log_signal);
    signal(SIGHUP, log_signal);

    while (1) {
        pause(); // Чекає сигналу
    }

    return 0;
}
