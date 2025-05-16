#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Робимо pipe неблокуючим
    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    // Створюємо великий буфер (1 MB)
    char *buffer = malloc(1024 * 1024);
    memset(buffer, 'A', 1024 * 1024);

    // Записуємо в pipe, в який нічого не читають (буфер заповниться)
    ssize_t count = write(pipefd[1], buffer, 1024 * 1024);

    if (count == -1) {
        perror("write");
    } else {
        printf("Requested 1MB, actually wrote %zd bytes\n", count);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    free(buffer);
    return 0;
}
