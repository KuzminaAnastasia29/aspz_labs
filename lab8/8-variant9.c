#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int i;

    for (i = 0; i < 3; i++) {
        printf("Iteration %d, PID: %d, Parent PID: %d\n", i, getpid(), getppid());
        fork();  // кожен процес створює ще один процес
        sleep(1); // затримка, щоб процеси не запускались миттєво
    }

    return 0;
}
