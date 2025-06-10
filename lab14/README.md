# Практичне заняття №14
## Example program – POSIX interval timers

## Код: `Example_program.c`
```
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void handler(int sig, siginfo_t *si, void *uc) {
    write(STDOUT_FILENO, "Tick\n", 5);
}

int main() {
    struct sigaction sa = {0};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigaction(SIGRTMIN, &sa, NULL);

    timer_t timerid;
    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    while (1)
        pause();
}
```
## Компіляція
```
cc -Wall -o Example_program Example_program.c
./Example_program
```

# Завдання по варіанту
Реалізуйте CLI-таймер, який виводить повідомлення через рівно N секунд без використання сигналів (через clock_nanosleep()).
## Опис виконання

Програма реалізує CLI-таймер, який приймає кількість секунд як аргумент командного рядка. Після перевірки правильності вводу, програма створює структуру timespec із заданим інтервалом і засинає на вказану кількість секунд за допомогою clock_nanosleep() без використання сигналів. Після завершення паузи виводиться повідомлення про закінчення таймера.

## Код: `variant9.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <секунди>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        fprintf(stderr, "Будь ласка, введіть додатне число секунд.\n");
        return EXIT_FAILURE;
    }

    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = 0;

    printf("Очікування %d секунд...\n", seconds);

    // Засипання без використання сигналів
    if (clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL) != 0) {
        perror("clock_nanosleep");
        return EXIT_FAILURE;
    }

    printf("Час вийшов! Минуло %d секунд.\n", seconds);
    return EXIT_SUCCESS;
}
```
## Компіляція
```
root@host:/home/guest/aspz_labs/lab12-13 # cc -Wall -o variant9 variant9.c
root@host:/home/guest/aspz_labs/lab12-13 # ./variant9

```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab14 # ./variant9
Використання: ./variant9 <секунди>
root@host:/home/guest/aspz_labs/lab14 # ./variant9 5
Очікування 5 секунд...
Час вийшов! Минуло 5 секунд.
```
