#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    pid_t pid = fork();

    if (pid == 0) {
        write(fd, "Child\n", 6);
    } else {
        write(fd, "Parent\n", 7);
    }

    close(fd);
    return 0;
}
