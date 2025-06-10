# Практичне заняття №10-11
## Використання системного виклику fork

## Код: `proc_fork.c`
```
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        printf("This is the child process\n");
    } else {
        printf("This is the parent process\n");
    }

    return 0;
}
```
## Компіляція
```
cc -Wall -o proc_fork proc_fork.c
./proc_fork
```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab10-11 # ./proc_fork
This is the parent process
This is the child process
```
## Експеримент: імітація роботи в процесах

## Код: `ex_fork.c`
```
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 5; i++) {
            printf("Child: %d\n", i);
            sleep(1);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            printf("Parent: %d\n", i);
            sleep(1);
        }
    }
    return 0;
}
```
## Компіляція
```
cc -Wall -o ex_fork ex_fork.c
./ex_fork
```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab10-11 # ./ex_fork
Parent: 0
Child: 0
Child: 1
Parent: 1
Parent: 2
Child: 2
Child: 3
Parent: 3
Parent: 4
Child: 4
```

## Копіювання даних при fork

## Код: `fork_copy.c`
```
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
```
## Компіляція
```
cc -Wall -o fork_copy fork_copy.c
./fork_copy
```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab10-11 # ./fork_copy
Child: counter = 1
Parent: counter = 0
```

## Процеси та відкриті файли

## Код `fork_files.c`

```
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

```
## Компіляція

```
cc -Wall -o fork_files fork_files.c
./fork_files
```

# Завдання по варіанту

## Опис виконання

Програма створює 10 дочірніх процесів у циклі за допомогою системного виклику fork(). PID кожного дочірнього процесу зберігається в масиві pids. Кожен дочірній процес одразу завершує виконання з кодом 0, а батьківський процес чекає на завершення кожного з них через wait(), після чого виводить інформацію про завершення процесу.

## Код: `variant9.c`

```
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
```
## Компіляція
```
root@host:/home/guest/aspz_labs/lab2 # cc -Wall -o variant9 variant9.c
root@host:/home/guest/aspz_labs/lab2 # ./variant9

```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab10-11 # ./variant9
Child 1 (PID: 1514) created
Child 2 (PID: 1515) created
Child 3 (PID: 1516) created
Child 4 (PID: 1517) created
Child 5 (PID: 1518) created
Child 6 (PID: 1519) created
Child 7 (PID: 1520) created
Child 8 (PID: 1521) created
Child 9 (PID: 1522) created
Child 10 (PID: 1523) created
Child with PID 1520 exited with status 0
Child with PID 1523 exited with status 0
Child with PID 1522 exited with status 0
Child with PID 1521 exited with status 0
Child with PID 1519 exited with status 0
Child with PID 1518 exited with status 0
Child with PID 1517 exited with status 0
Child with PID 1516 exited with status 0
Child with PID 1515 exited with status 0
Child with PID 1514 exited with status 0
```
