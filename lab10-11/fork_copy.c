#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0;
    pid_t pid = fork();

    if (pid == 0) {
        counter++;
        printf("Child: counter = %d\n", counter);
    } else {
        sleep(1);
        printf("Parent: counter = %d\n", counter);
    }

    return 0;
}
