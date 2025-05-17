# Практична робота 7
## Завдання 7.1 

## Формулювання завдання

Використайте `popen()`, щоб передати вивід команди `rwho` (команда UNIX) до `more` (команда UNIX) у програмі на C.
## Опис виконання

У програмі відкривається два потоки: один для команди `rwho`, інший — для `more`. Вивід rwho перенаправляється на вхід more через канал, реалізований через `popen()`. Це дозволяє показати вивід команди rwho посторінково, як це робить утиліта more.
## Код: `7-1.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *rwho_fp, *more_fp;
    char buffer[256];

    rwho_fp = popen("rwho", "r");
    if (!rwho_fp) {
        perror("popen rwho");
        return 1;
    }

    more_fp = popen("more", "w");
    if (!more_fp) {
        perror("popen more");
        pclose(rwho_fp);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), rwho_fp)) {
        fputs(buffer, more_fp);
    }

    pclose(rwho_fp);
    pclose(more_fp);

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-1 7-1.c
root@host:/home/guest/labs/lab7 # ./7-1
```
## Завдання 7.2

## Формулювання завдання

Напишіть програму мовою C, яка імітує команду `ls -l` в UNIX — виводить список усіх файлів у поточному каталозі та перелічує права доступу тощо.
 (Варіант вирішення, що просто виконує `ls -l` із вашої програми, — не підходить.)

## Опис виконання

Програма відкриває поточний каталог, перебирає усі файли, і для кожного виконує `stat()`, щоб отримати інформацію про права, власника, групу, розмір і дату останньої модифікації. Формат виводу повторює `ls -l`.
## Код: `7-2.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void print_permissions(mode_t mode) {
    char perms[11] = "----------";

    if (S_ISDIR(mode)) perms[0] = 'd';
    else if (S_ISLNK(mode)) perms[0] = 'l';
    else if (S_ISCHR(mode)) perms[0] = 'c';
    else if (S_ISBLK(mode)) perms[0] = 'b';
    else if (S_ISFIFO(mode)) perms[0] = 'p';
    else if (S_ISSOCK(mode)) perms[0] = 's';

    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';

    printf("%s", perms);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char time_str[64];

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        print_permissions(file_stat.st_mode);
        printf(" %ld", (long)file_stat.st_nlink);

        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);

        printf(" %s", pw ? pw->pw_name : "unknown");
        printf(" %s", gr ? gr->gr_name : "unknown");

        printf(" %5ld", (long)file_stat.st_size);

        struct tm *tm_info = localtime(&file_stat.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", tm_info);
        printf(" %s", time_str);

        printf(" %s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-2 7-2.c
root@host:/home/guest/labs/lab7 # ./7-2
-rw-r--r-- 1 root guest   823 May 16 18:31 7-1.c
-rw-r--r-- 1 root guest  2071 May 16 19:49 7-2.c
-rwxr-xr-x 1 root guest 10656 May 17 10:02 7-1
-rw-r--r-- 1 root guest   673 May 16 20:07 7-3.c
-rwxr-xr-x 1 root guest 12360 May 17 10:03 7-2
-rw-r--r-- 1 root guest   574 May 16 20:38 text.txt
-rwxr-xr-x 1 root guest 11168 May 16 20:09 7-3
-rw-r--r-- 1 root guest   996 May 16 20:47 7-4.c
-rw-r--r-- 1 root guest  3090 May 16 20:52 text2.txt
-rw-r--r-- 1 root guest  1038 May 16 21:02 7-5.c
-rw-r--r-- 1 root guest  1210 May 16 21:27 7-6.c
-rwxr-xr-x 1 root guest 12144 May 16 21:02 7-4
-rwxr-xr-x 1 root guest 11064 May 16 21:03 7-5
drwxr-xr-x 2 root guest   512 May 16 21:28 test1
-rwxr-xr-x 1 root guest 11544 May 16 21:27 7-6
-rw-r--r-- 1 root guest  1729 May 16 21:32 7-7.c
-rw-r--r-- 1 root guest  1232 May 16 21:35 7-8.c
-rwxr-xr-x 1 root guest 12096 May 16 21:33 7-7
-rw-r--r-- 1 root guest   906 May 16 21:37 7-9.c
-rwxr-xr-x 1 root guest 11448 May 16 21:36 7-8
-rw-r--r-- 1 root guest  1328 May 16 21:40 7-10.c
-rwxr-xr-x 1 root guest 10208 May 16 21:38 7-9
-rw-r--r-- 1 root guest   834 May 16 21:46 7-variant9.c
-rwxr-xr-x 1 root guest 11472 May 16 21:40 7-10
-rwxr-xr-x 1 root guest 11064 May 16 21:46 7-variant9
```
## Завдання 7.3

## Формулювання завдання

Напишіть програму, яка друкує рядки з файлу, що містять слово, передане як аргумент програми (проста версія утиліти grep в UNIX).
## Опис виконання

Програма приймає слово для пошуку та ім'я файлу, відкриває файл, построчно читає і виводить тільки ті рядки, де знайдено вказане слово.
## Код: `7-3.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Використання: %s <слово> <файл>\n", argv[0]);
        return 1;
    }

    const char *word = argv[1];
    FILE *file = fopen(argv[2], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, word)) {
            printf("%s", line);
        }
    }

    fclose(file);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-3 7-3.c
root@host:/home/guest/labs/lab7 # ./7-3 Lorem text.txt
Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions o
```
## Завдання 7.4 

## Формулювання завдання

Напишіть програму, яка виводить список файлів, заданих у вигляді аргументів, з зупинкою кожні 20 рядків, доки не буде натиснута клавіша (спрощена версія утиліти more в UNIX).
## Опис виконання

Було реалізовано програму, яка:

Приймає список імен файлів як аргументи командного рядка.

Для кожного файлу:

Виводить заголовок з ім’ям файлу.

Зчитує файл построчно.

Після кожних 20 рядків зупиняється та чекає, поки користувач натисне клавішу Enter.

Обробляє помилки відкриття файлів, виводячи відповідне повідомлення.

Ця програма дозволяє переглядати великі текстові файли у зручному інтерактивному режимі, що особливо корисно для виводу логів або звітів у терміналі без прокручування.

Особливості реалізації
Кожен файл обробляється окремо, і перед його вмістом виводиться назва файлу у вигляді заголовка `filename`.

Для введення клавіші використовується `getchar()`, що дозволяє призупинити виконання до підтвердження користувача.

Вивід організований по сторінках (по 20 рядків), як у класичних UNIX-утилітах.

Використовується буфер довжиною 1024 символи для безпечного зчитування рядків.
## Код: `7-4.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_file_with_pause(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return;
    }

    printf("=== %s ===\n", filename);
    char line[1024];
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        line_count++;

        if (line_count % 20 == 0) {
            printf("-- Натисніть Enter для продовження --");
            while (getchar() != '\n');  // Очікуємо натискання Enter
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Використання: %s <файл1> [файл2] ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file_with_pause(argv[i]);
    }

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-4 7-4.c
root@host:/home/guest/labs/lab7 # ./7-4 text.txt text2.txt
```


## Завдання 7.5

## Формулювання завдання

Напишіть програму, яка перелічує всі файли в поточному каталозі та всі файли в підкаталогах.
## Опис виконання

Реалізовано програму, яка:

Починає обхід із поточного робочого каталогу (.).

Рекурсивно входить у кожен підкаталог, ігноруючи . та ...

Для кожного знайденого файлу виводить повний (відносний) шлях до нього.

Для перевірки, чи є об’єкт директорією, використовується функція ‘stat()’ та перевірка прапора ‘S_ISDIR’.

Використовується глибокий обхід файлової системи, що дозволяє переглянути всю структуру каталогів і файлів.

Особливості реалізації
Для навігації по файловій системі використано функції з ‘dirent.h’: ‘opendir()’, ‘readdir()’, ‘closedir()’.

Відносні шляхи будуються шляхом об’єднання імен поточного каталогу з іменами знайдених файлів.

Враховано обробку помилок при відкритті каталогу чи виклику ‘stat()’.

## Код: `7-5.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаємо . та ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1) {
            perror(full_path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            list_files(full_path);  // рекурсивно переходимо у підкаталог
        } else {
            printf("%s\n", full_path);
        }
    }

    closedir(dir);
}

int main() {
    list_files(".");
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # ./7-5

.:
7-1.c
7-2.c
7-1
7-3.c
7-2
text.txt
7-3
7-4.c
text2.txt
7-5.c
7-6.c
7-4
7-5
test1

./test1:
text3.txt
7-6
7-7.c
7-8.c
7-7
7-9.c
7-8
7-10.c
7-9
7-variant9.c
7-10
7-variant9
```


## Завдання 7.6

## Формулювання завдання

Напишіть програму, яка перелічує лише підкаталоги у алфавітному порядку.
## Опис виконання

У цьому завданні було реалізовано програму, яка виводить список лише підкаталогів поточного каталогу у алфавітному порядку. Спочатку відкривається поточний робочий каталог за допомогою функції ‘opendir()’. Далі за допомогою ‘readdir()’ зчитуються всі записи в каталозі. Кожен запис перевіряється: якщо це не . або .., програма викликає ‘stat()’, щоб визначити тип файлу. Якщо файл є директорією (підкаталогом), його назва додається до масиву.

Після завершення зчитування всі зібрані назви підкаталогів сортуються у алфавітному порядку з використанням функції ‘qsort()’. Для сортування застосовується простий компаратор, який порівнює рядки за допомогою ‘strcmp()’. Нарешті, відсортовані назви підкаталогів виводяться на екран по одному в рядку. Завдяки такому підходу користувач отримує повний і впорядкований список підкаталогів у зручному для перегляду вигляді.
## Код: `7-6.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_DIRS 1024

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char *dirs[MAX_DIRS];
    int count = 0;

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &st) == 0 && S_ISDIR(st.st_mode)) {
            dirs[count] = strdup(entry->d_name);
            count++;
        }
    }

    closedir(dir);

    qsort(dirs, count, sizeof(char *), compare);

    printf("Підкаталоги у алфавітному порядку:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", dirs[i]);
        free(dirs[i]);
    }

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-6 7-6.c
root@host:/home/guest/labs/lab7 # ./7-6
test1
```
## Завдання 7.7

## Формулювання завдання

Напишіть програму, яка показує користувачу всі його/її вихідні програми на C, а потім в інтерактивному режимі запитує, чи потрібно надати іншим дозвіл на читання ‘(read permission)’; у разі ствердної відповіді — такий дозвіл повинен бути наданий.
## Опис виконання

У цьому завданні було створено програму, яка автоматично визначає всі виконувані файли C-програм у поточному каталозі, після чого взаємодіє з користувачем у інтерактивному режимі. Спочатку програма відкриває поточний каталог і переглядає всі його елементи. Для кожного файлу вона перевіряє, чи є він виконуваним, тобто чи має відповідні права доступу ‘(X_OK)’ і чи не є директорією. Якщо файл виконується, його вважають потенційною C-програмою.

Кожен такий файл виводиться на екран з відповідним запитом — чи слід надати дозвіл на читання для інших користувачів. В залежності від відповіді користувача ‘(y/n)’, програма або змінює права доступу, додаючи до них ‘S_IROTH’, або переходить до наступного файлу. Таким чином, реалізовано простий механізм контролю над доступом до виконуваних файлів безпосередньо з консолі, що дає змогу зручно керувати правами на читання у багатокористувацькому середовищі.
## Код: `7-7.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int is_executable(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == -1) return 0;
    return S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR);
}

int main() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    printf("Ваші виконувані C-програми:\n");

    while ((entry = readdir(dir)) != NULL) {
        if (is_executable(entry->d_name)) {
            printf("Файл: %s\n", entry->d_name);
            printf("Надати дозвіл на читання для інших користувачів? (y/n): ");
            char response;
            scanf(" %c", &response);
            if (response == 'y' || response == 'Y') {
                struct stat st;
                if (stat(entry->d_name, &st) == 0) {
                    mode_t new_mode = st.st_mode | S_IROTH;
                    if (chmod(entry->d_name, new_mode) == 0) {
                        printf("Дозвіл надано \n");
                    } else {
                        perror("chmod");
                    }
                }
            }
        }
    }

    closedir(dir);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-7 7-7.c
root@host:/home/guest/labs/lab7 # ./7-7
Ваші виконувані C-програми:
Файл: 7-1
Надати дозвіл на читання для інших користувачів? (y/n): y
Дозвіл надано 
Файл: 7-2
Надати дозвіл на читання для інших користувачів? (y/n): n
Файл: 7-3
Надати дозвіл на читання для інших користувачів? (y/n): n
Файл: 7-4
Надати дозвіл на читання для інших користувачів? (y/n): y
Дозвіл надано 
Файл: 7-5
Надати дозвіл на читання для інших користувачів? (y/n): n
Файл: 7-6
Надати дозвіл на читання для інших користувачів? (y/n): y
Дозвіл надано 
Файл: 7-7
Надати дозвіл на читання для інших користувачів? (y/n): n
Файл: 7-8
Надати дозвіл на читання для інших користувачів? (y/n): y
Дозвіл надано 
Файл: 7-9
Надати дозвіл на читання для інших користувачів? (y/n): n
Файл: 7-10
Надати дозвіл на читання для інших користувачів? (y/n): y
Дозвіл надано 
Файл: 7-variant9
Надати дозвіл на читання для інших користувачів? (y/n): n
```
## Завдання 7.8

## Формулювання завдання

Напишіть програму, яка надає користувачу можливість видалити будь-який або всі файли у поточному робочому каталозі. Має з’являтися ім’я файлу з запитом, чи слід його видалити.
## Опис виконання

У цьому завданні реалізовано програму, що сканує поточний каталог і для кожного звичайного файла (тобто не каталогу) задає користувачу запит, чи потрібно його видалити. Програма відкриває поточну директорію, переглядає всі її вміст, ігноруючи спеціальні записи "." та "..".

Якщо знайдено файл, користувачу виводиться його назва разом із запитанням ‘(y/n)’, чи слід його видалити. У разі підтвердження ‘(y)’, програма виконує видалення файлу за допомогою ‘remove()’, а потім повідомляє про успіх або помилку. Якщо користувач вводить ‘n’, програма переходить до наступного файлу без дій. Таким чином, реалізовано простий та ефективний інструмент ручного контролю за вмістом поточного каталогу з інтерфейсом у стилі командного рядка.
## Код: `7-8.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ігнорувати "." та ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode)) {
            printf("Файл: %s\n", entry->d_name);
            printf("Видалити? (y/n): ");
            char response;
            scanf(" %c", &response);
            if (response == 'y' || response == 'Y') {
                if (remove(entry->d_name) == 0) {
                    printf("Файл видалено \n");
                } else {
                    perror("remove");
                }
            }
        }
    }

    closedir(dir);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-8 7-8.c
root@host:/home/guest/labs/lab7 # ./7-8
Файл: 7-1.c — Видалити? (y/n): n
Файл: 7-2.c — Видалити? (y/n): n
Файл: 7-1 — Видалити? (y/n): n
Файл: 7-3.c — Видалити? (y/n): n
Файл: 7-2 — Видалити? (y/n): n
Файл: text.txt — Видалити? (y/n): n
Файл: 7-3 — Видалити? (y/n): n
Файл: 7-4.c — Видалити? (y/n): n
Файл: text2.txt — Видалити? (y/n): n
Файл: 7-5.c — Видалити? (y/n): n
Файл: 7-6.c — Видалити? (y/n): n
Файл: 7-4 — Видалити? (y/n): n
Файл: 7-5 — Видалити? (y/n): n
Файл: 7-6 — Видалити? (y/n): n
Файл: 7-7.c — Видалити? (y/n): n
Файл: 7-8.c — Видалити? (y/n): n
Файл: 7-7 — Видалити? (y/n): n
Файл: 7-9.c — Видалити? (y/n): n
Файл: 7-8 — Видалити? (y/n): n
Файл: 7-10.c — Видалити? (y/n): n
Файл: 7-9 — Видалити? (y/n): n
Файл: 7-variant9.c — Видалити? (y/n): n
Файл: 7-10 — Видалити? (y/n): n
Файл: 7-variant9 — Видалити? (y/n): n
```


## Завдання 7.9

## Формулювання завдання

Напишіть програму на C, яка вимірює час виконання фрагмента коду в мілісекундах.
## Опис виконання

У цьому завданні реалізовано програму, яка вимірює тривалість виконання певної ділянки коду з точністю до мілісекунд. Для цього використовується функція ‘gettimeofday()’, що дозволяє отримати поточний час із мікросекундною точністю.

Програма зберігає час перед початком виконання обчислень і після їх завершення, після чого обчислює різницю між двома часовими позначками та виводить її у мілісекундах.
Як приклад навантаження в код вставлено простий цикл із затримкою, щоб було видно різницю в часі. Можна замінити цей блок на будь-який інший фрагмент, час виконання якого потрібно виміряти.
## Код: `7-9.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;

    gettimeofday(&start, NULL);

    // Початок вимірюваного блоку
    for (volatile int i = 0; i < 10000000; i++);
    // Кінець вимірюваного блоку

    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;      // сек → мс
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;          // мкс → мс

    printf("Час виконання: %.3f мс\n", elapsed);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-9 7-9.c
root@host:/home/guest/labs/lab7 # ./7-9
Час виконання: 78.275 мс
```
## Завдання 7.10

## Формулювання завдання

Напишіть програму мовою C для створення послідовності випадкових чисел з плаваючою комою у діапазонах:
 (a) від 0.0 до 1.0
 (b) від 0.0 до n, де n — будь-яке дійсне число з плаваючою точкою.
 Початкове значення генератора випадкових чисел має бути встановлене так, щоб гарантувати унікальну послідовність.
Примітка: використання прапорця -Wall під час компіляції є обов’язковим.
## Опис виконання

Було реалізовано програму, яка приймає одне числове значення з плаваючою точкою як аргумент командного рядка. Програма генерує 10 випадкових чисел типу ‘float’ у діапазоні від 0.0 до цього значення. Якщо аргумент не передано, за замовчуванням використовується межа 1.0.

Для генерації чисел використовується функція rand(), а для забезпечення унікальності послідовності на кожен запуск — ‘srand(time(NULL))’.
## Код: `7-10.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    float max = 1.0f;

    if (argc > 1) {
        max = atof(argv[1]);
        if (max <= 0.0f) {
            fprintf(stderr, "Будь ласка, введіть число більше нуля\n");
            return 1;
        }
    }

    srand((unsigned int)time(NULL));

    printf("Генерація 10 випадкових чисел у діапазоні від 0.0 до %.3f:\n", max);
    for (int i = 0; i < 10; i++) {
        float rnd = ((float)rand() / RAND_MAX) * max;
        printf("%.6f\n", rnd);
    }

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-10 7-10.c
root@host:/home/guest/labs/lab7 # ./7-10 1.0
Генерація 10 випадкових чисел у діапазоні від 0.0 до 1.000:
0.757436
0.588694
0.600127
0.878250
0.577892
0.242649
0.021155
0.811531
0.852584
0.619496
```


## Завдання 7.variant9

## Формулювання завдання

Розробіть сканер портів у локальній мережі, який не використовує сокети або будь-які мережеві бібліотеки напряму.
## Опис виконання

Було реалізовано просту програму на мові C, яка перевіряє доступність певного порту на IP-адресі, не використовуючи сокети. Замість цього застосовується виклик ‘popen()’ для виконання системної утиліти (наприклад, ‘nc’ або ‘telnet’) у підпроцесі.

Вивід команди аналізується через ‘strstr()’ — шукається успішне підключення (ключові слова: ‘"open"’ або ‘"succeeded"’).
Якщо такі слова знайдено, порт вважається відкритим. Інакше — закритим.

Цей підхід дозволяє обійти використання ‘socket()’, але вимагає наявності відповідних команд у системі.
## Код: `7-variant9.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *ip = "192.168.1.1";
    int port = 80;
    char command[256];
    char output[512];

    snprintf(command, sizeof(command), "nc -zv %s %d 2>&1", ip, port);

    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen");
        return 1;
    }

    while (fgets(output, sizeof(output), fp)) {
        if (strstr(output, "succeeded") != NULL || strstr(output, "open") != NULL) {
            printf("Порт %d на %s відкритий\n", port, ip);
            pclose(fp);
            return 0;
        }
    }

    printf("Порт %d на %s закритий\n", port, ip);
    pclose(fp);
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab7 # cc -Wall -o 7-variant9 7-variant9.c
root@host:/home/guest/labs/lab7 # ./7-variant9
Порт 80 на 192.168.1.1 закритий
```
