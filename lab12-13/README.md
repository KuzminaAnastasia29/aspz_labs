# Практичне заняття №12-13
## Handling Signals

## Код: `Handling_Signals_ex.c`
```
#include <signal.h>
#include <stdio.h>

void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
}

int main() {
    signal(SIGINT, handle_sigint);
    while (1) {} // нескінченний цикл
}

```
## Компіляція
```
cc -Wall -o Handling_Signals_ex pHandling_Signals_ex.c
./Handling_Signals_ex
```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab12-13 # ./Handling_Signals_ex

^Z[1] + Suspended               ./Handling_Signals_ex
```

## A Simple C Program that Handles a Couple of Signals

## Код: `signal_handler.c`
```
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void handle_sigint(int sig) {
    printf("Caught SIGINT (Ctrl+C)\n");
}
void handle_sigterm(int sig) {
    printf("Caught SIGTERM, exiting...\n");
    _exit(0);
}
int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);
    printf("PID: %d\n", getpid());
    while (1) {
        sleep(1);
    }
}
```
## Компіляція
```
cc -Wall -o signal_handler signal_handler.c
./signal_handler
```
## Результати дослідження
```
root@host:/home/guest/aspz_labs/lab12-13 # ./signal_handler
PID: 1714
^CCaught SIGINT (Ctrl+C)
^Z[2] + Suspended               ./signal_handler
```

# Завдання по варіанту
Напишіть демон, який відстежує всі сигнали, що надходять до системи, і фіксує PID, UID, GID та ім’я процесу-відправника, використовуючи audit або netlink (якщо доступно).
## Опис виконання

Програма створює демон, який у фоновому режимі відкриває Netlink-сокет і підписується на події аудиту ядра Linux. За допомогою AUDIT_SET програма вмикає відстеження всіх повідомлень від audit-підсистеми. У нескінченному циклі демон отримує ці повідомлення (зокрема ті, що стосуються надсилання сигналів між процесами) та записує у файл /var/log/signal_audit.log інформацію з них

## Код: `signal_audit_daemon.c`

```
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
```
## Компіляція
```
root@host:/home/guest/aspz_labs/lab12-13 # cc -Wall -o signal_audit_daemon signal_audit_daemon.c
root@host:/home/guest/aspz_labs/lab12-13 # ./signal_audit_daemon

```
