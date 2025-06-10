#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILDREN 10

int main() {
    pid_t pids[NUM_CHILDREN];

    // Створення дочірніх процесів
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Дочірній процес
            printf("Child %d (PID: %d) created\n", i + 1, getpid());
            exit(0);  // Завершення дочірнього процесу
        }
        else {
            // Батьківський процес
            pids[i] = pid;
        }
    }

    // Очікування завершення дочірніх процесів
    for (int i = 0; i < NUM_CHILDREN; i++) {
        int status;
        pid_t finished = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child with PID %d exited with status %d\n", finished, WEXITSTATUS(status));
        } else {
            printf("Child with PID %d did not exit normally\n", finished);
        }
    }

    return 0;
}
