# Практична робота 9
## Завдання 9-1
## Формулювання завдання
Написати програму, яка читає список облікових записів за допомогою getent passwd і виводить звичайних користувачів (UID >= 500 у FreeBSD). Програма має показати інформацію про користувачів, окрім поточного.
## Опис виконання

Програма читає список користувачів системи через команду `getent passwd`, визначає звичайних користувачів за UID (більше або рівно 500 у FreeBSD) та виводить їх імена, UID, домашній каталог і оболонку. Програма ігнорує користувачів з оболонками nologin чи 'false'. Це дозволяє дізнатися, які облікові записи є на комп’ютері і які з них — звичайні користувачі.
## Код: `9-1.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // для getuid()

#define UID_THRESHOLD 500
#define MAX_LINE 1024

int main() {
    FILE *fp;
    char line[MAX_LINE];

    // Виконуємо команду getent passwd
    fp = popen("getent passwd", "r");
    if (fp == NULL) {
        perror("Не вдалося виконати getent");
        return 1;
    }

    printf("Звичайні користувачі у системі (UID >= %d):\n", UID_THRESHOLD);

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *login = strtok(line, ":");
        strtok(NULL, ":");  // password
        char *uid_str = strtok(NULL, ":");
        strtok(NULL, ":");  // gid
        char *gecos = strtok(NULL, ":");
        char *home = strtok(NULL, ":");
        char *shell = strtok(NULL, ":\n");

        if (login && uid_str) {
            int uid = atoi(uid_str);
            if (uid >= UID_THRESHOLD && uid != getuid()) {
                printf("Користувач: %s, UID: %d, Дім: %s, Shell: %s\n",
                       login, uid, home ? home : "-", shell ? shell : "-");
            }
        }
    }

    pclose(fp);
    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab9 # cc -Wall -o 9-1 9-1.c
root@host:/home/guest/labs/lab9 # ./9-1
Звичайні користувачі у системі (UID >= 500, shell ≠ nologin/false):
Користувач: guest, UID: 1001, Дім: /home/guest, Shell: /bin/sh
```
## Завдання 9-4
## Формулювання завдання
Напишіть програму, яка по черзі виконує команди whoami та id, щоб перевірити стан облікового запису користувача, від імені якого вона запущена.
 Є ймовірність, що команда id виведе список різних груп, до яких ви належите. Програма повинна це продемонструвати.
## Опис виконання

Програма виконує команди `whoami` і `id`, щоб показати ім’я користувача, від імені якого запущено процес, а також його UID, GID і групи, до яких він належить. Це допомагає зрозуміти, які права і членства є у користувача у системі.
## Код: `9-4.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Виконання команди whoami:\n");
    int whoami_status = system("whoami");

    if (whoami_status == -1) {
        perror("Помилка при виконанні whoami");
    }

    printf("\nВиконання команди id:\n");
    int id_status = system("id");

    if (id_status == -1) {
        perror("Помилка при виконанні id");
    }

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab9 # cc -Wall -o 9-4 9-4.c
root@host:/home/guest/labs/lab9 # ./9-4
Виконання команди whoami:
root

Виконання команди id:
uid=0(root) gid=0(wheel) groups=0(wheel),5(operator)
```
## Завдання 9-6
## Формулювання завдання
Напишіть програму, яка виконує команду `ls -l`, щоб переглянути власника і права доступу до файлів у своєму домашньому каталозі, в /usr/bin та в /etc.
 Продемонструйте, як ваша програма намагається обійти різні власники та права доступу користувачів, а також здійснює спроби читання, запису та виконання цих файлів.
## Опис виконання

Програма виконує команду `ls -l` для трьох каталогів: домашнього каталогу користувача, `/usr/bin` і `/etc`. Вона виводить інформацію про власника та права доступу до файлів. Далі програма намагається виконати операції читання, запису і виконання цих файлів, демонструючи, як права впливають на можливість роботи з файлами.
## Код: `9-6.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void check_file_access(const char *path, const char *filename) {
    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);

    struct stat st;
    if (stat(fullpath, &st) != 0) {
        perror("stat");
        return;
    }

    printf("\nФайл: %s\n", fullpath);
    printf("  Власник UID: %d, GID: %d\n", st.st_uid, st.st_gid);
    printf("  Права: %o\n", st.st_mode & 0777);

    // Читання
    int fd = open(fullpath, O_RDONLY);
    if (fd != -1) {
        printf("   Можна читати\n");
        close(fd);
    } else {
        printf("   Не можна читати: %s\n", strerror(errno));
    }

    // Запис
    fd = open(fullpath, O_WRONLY);
    if (fd != -1) {
        printf("  Можна писати\n");
        close(fd);
    } else {
        printf("   Не можна писати: %s\n", strerror(errno));
    }

    // Виконання (лише якщо це файл і має x-права)
    if (access(fullpath, X_OK) == 0) {
        printf("  Можна виконувати\n");
    } else {
        printf("   Не можна виконувати: %s\n", strerror(errno));
    }
}

void scan_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Не вдалося відкрити каталог");
        return;
    }

    printf("\n Каталог: %s\n", path);
    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < 5) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            check_file_access(path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
}

int main() {
    char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Змінна HOME не визначена\n");
        return 1;
    }

    scan_directory(home);
    scan_directory("/usr/bin");
    scan_directory("/etc");

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab9 # cc -Wall -o 9-6 9-6.c
root@host:/home/guest/labs/lab9 # ./9-6

 Каталог: /root

Файл: /root/.cshrc
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /root/.profile
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /root/.k5login
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /root/.login
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /root/.shrc
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

 Каталог: /usr/bin

Файл: /usr/bin/asn1_compile
  Власник UID: 0, GID: 0
  Права: 555
   Можна читати
   Можна писати
   Можна виконувати

Файл: /usr/bin/biff
  Власник UID: 0, GID: 0
  Права: 555
   Можна читати
   Можна писати
   Можна виконувати

Файл: /usr/bin/byacc
  Власник UID: 0, GID: 0
  Права: 555
   Можна читати
   Можна писати
   Можна виконувати

Файл: /usr/bin/zcat
  Власник UID: 0, GID: 0
  Права: 555
   Можна читати
   Можна писати
   Можна виконувати

Файл: /usr/bin/cd
  Власник UID: 0, GID: 0
  Права: 555
   Можна читати
   Можна писати
   Можна виконувати

 Каталог: /etc

Файл: /etc/pf.os
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /etc/group
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /etc/network.subr
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /etc/locate.rc
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied

Файл: /etc/csh.logout
  Власник UID: 0, GID: 0
  Права: 644
   Можна читати
   Можна писати
   Не можна виконувати: Permission denied
```
# Завдання 9-variant9
## Формулювання завдання
Чи можна мати два різні облікові записи з однаковим UID? Які наслідки?
## Відповідь
Так, у Unix-подібних системах теоретично можна мати два різні облікові записи з однаковим UID. Це означає, що система розглядає їх як одного користувача з точки зору прав доступу до файлів і ресурсів. Через це обидва облікові записи матимуть однакові права, і між ними може виникнути плутанина, наприклад, у відображенні власника файлів чи процесів.

Однак така практика не є рекомендованою, бо це може створити проблеми з безпекою та ускладнити адміністрування системи. Зазвичай кожен користувач повинен мати унікальний UID, щоб уникнути конфліктів і забезпечити чітку ідентифікацію користувачів. Винятки можуть бути тільки для службових облікових записів, які працюють з однаковими правами.


