#Практична робота №3
#Завдання 3
# Імітація кидання шестигранного кубика з обмеженням на розмір файлу

## Опис завдання

Програма імітує кидання стандартного шестигранного кубика (з гранями від 1 до 6). Результати кожного кидка записуються у текстовий файл під назвою dice_rolls.txt. Перед кожним записом програма перевіряє поточний розмір файлу і припиняє запис нових результатів, якщо досягнуто встановленого максимального розміру файлу (за замовчуванням 1024 байти). У випадку досягнення ліміту програма виводить відповідне повідомлення на екран.

## Код програми (C)
```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#define FILENAME "dice_rolls.txt"
#define MAX_FILE_SIZE 1024  // Обмеження у байтах (наприклад, 1 КБ)

int get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return 0;
}

int main() {
    FILE *file;
    int roll;
    int file_size;

    // Ініціалізація генератора випадкових чисел
    srand(time(NULL));

    // Відкриття файлу для дозапису
    file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Не вдалося відкрити файл");
        return 1;
    }

    while (1) {
        file_size = get_file_size(FILENAME);
        if (file_size >= MAX_FILE_SIZE) {
            printf("Досягнуто максимального розміру файлу (%d байт).\n", MAX_FILE_SIZE);
            break;
        }

        // Імітація кидка кубика (від 1 до 6)
        roll = rand() % 6 + 1;

        // Запис результату у файл
        fprintf(file, "%d\n", roll);
    }

    fclose(file);
    return 0;
}
```
## Компіляція та запуск у FreeBSD
```
root@host:/home/guest/aspz_labs/lab3 # ./3-3
Досягнуто максимального розміру файлу (1024 байт).
```

#Завдання 4

## Опис завдання

Програма імітує процес лотереї, генеруючи дві послідовності унікальних випадкових чисел: сім різних цілих чисел у діапазоні від 1 до 49, та шість різних цілих чисел у діапазоні від 1 до 36. Для демонстрації обробки обмежень ресурсів, для програми встановлюється обмеження на максимальний час використання центрального процесора (CPU time). У випадку, якщо програма перевищує встановлений ліміт часу ЦП, вона коректно завершує свою роботу та виводить відповідне повідомлення.

## Код програми (C)
```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>

#define COUNT_49 7
#define MAX_49 49
#define COUNT_36 6
#define MAX_36 36

void handle_cpu_limit(int sig) {
    printf("\n[!] Ліміт часу ЦП вичерпано. Завершення програми.\n");
    exit(1);
}

void generate_unique_numbers(int *array, int count, int max) {
    int num, i, exists;

    for (i = 0; i < count;) {
        num = rand() % max + 1;
        exists = 0;
        for (int j = 0; j < i; j++) {
            if (array[j] == num) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            array[i++] = num;
        }
    }
}

void print_numbers(int *array, int count, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    // Встановлення обмеження часу ЦП: 1 секунда
    struct rlimit rl;
    rl.rlim_cur = 1;  // поточне обмеження
    rl.rlim_max = 1;  // максимальне обмеження
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        perror("Не вдалося встановити обмеження часу ЦП");
        return 1;
    }

    // Обробник сигналу перевищення часу ЦП
    signal(SIGXCPU, handle_cpu_limit);

    // Ініціалізація генератора випадкових чисел
    srand(time(NULL));

    int numbers_49[COUNT_49];
    int numbers_36[COUNT_36];

    generate_unique_numbers(numbers_49, COUNT_49, MAX_49);
    generate_unique_numbers(numbers_36, COUNT_36, MAX_36);

    print_numbers(numbers_49, COUNT_49, "Числа (7 з 49)");
    print_numbers(numbers_36, COUNT_36, "Числа (6 з 36)");

    return 0;
}
```
## Компіляція та запуск у FreeBSD
```
root@host:/home/guest/aspz_labs/lab3 # ./3-4
Числа (7 з 49): 37 42 29 46 47 40 9 
Числа (6 з 36): 15 4 23 13 9 10 
```
#Завдання 5
# Програма для копіювання файлів з перевіркою аргументів та обмеженням розміру

## Опис завдання

Ця програма призначена для копіювання вмісту одного файлу (джерела) в інший файл (призначення). Імена файлів джерела та призначення передаються програмі як аргументи командного рядка. Програма виконує наступні перевірки та дії:

* Перевіряє, чи було передано рівно два аргументи (імена файлів). Якщо кількість аргументів не відповідає двом, програма виводить повідомлення "Program need two arguments" і завершується з кодом помилки.
* Перевіряє, чи доступний файл джерела для читання. Якщо файл не може бути відкритий для читання, програма виводить повідомлення "Cannot open file \[ім'я\_файлу] for reading" разом з описом помилки та завершується з кодом помилки.
* Перевіряє, чи доступний файл призначення для запису. Якщо файл не може бути відкритий для запису, програма виводить повідомлення "Cannot open file \[ім'я\_файлу] for writing" разом з описом помилки, закриває вже відкритий файл джерела (якщо він був відкритий) та завершується з кодом помилки.
* Обробляє ситуацію перевищення обмеження на розмір вихідного файлу. Хоча в наданому коді обробник сигналу SIGXFSZ встановлено, фактичне обмеження на розмір файлу не налаштовується через setrlimit(RLIMIT_FSIZE, ...). Тому, щоб активувати обробку перевищення розміру файлу, необхідно додатково встановити відповідне обмеження на рівні системи або процесу. У випадку отримання сигналу SIGXFSZ (що сигналізує про перевищення ліміту розміру файлу), програма виведе повідомлення "Exceeded file size limit. Aborting copy." та завершиться.
* Успішно копіює вміст файлу джерела до файлу призначення, використовуючи буфер розміром 4 КБ. Після успішного копіювання програма виводить повідомлення "File copied successfully.".

## Код програми (C)
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096  // 4 КБ

void handle_file_size_limit(int sig) {
    printf("Exceeded file size limit. Aborting copy.\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    const char *source = argv[1];
    const char *target = argv[2];

    // Встановлення обробника сигналу перевищення розміру файлу
    signal(SIGXFSZ, handle_file_size_limit);

    // Відкриття вхідного файлу для читання
    FILE *in = fopen(source, "rb");
    if (!in) {
        fprintf(stderr, "Cannot open file %s for reading: %s\n", source, strerror(errno));
        return 1;
    }

    // Відкриття вихідного файлу для запису
    FILE *out = fopen(target, "wb");
    if (!out) {
        fprintf(stderr, "Cannot open file %s for writing: %s\n", target, strerror(errno));
        fclose(in);
        return 1;
    }

    // Копіювання вмісту
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
        if (fwrite(buffer, 1, bytesRead, out) != bytesRead) {
            fprintf(stderr, "Write error or file size limit exceeded\n");
            fclose(in);
            fclose(out);
            return 1;
        }
    }

    fclose(in);
    fclose(out);

    printf("File copied successfully.\n");
    return 0;
}
```
## Компіляція та запуск у FreeBSD
```
root@host:/home/guest/aspz_labs/lab3 # ./3-5
Program need two arguments
```

#Завдання 6
# Демонстрація обмеження розміру стеку

## Опис завдання

Ця програма демонструє вплив обмеження на розмір стекового сегменту пам'яті. Вона використовує рекурсивну функцію recursiveFunction, яка на кожному рівні свого виклику виділяє на стеку буфер розміром 1 КБ. Програма продовжує рекурсивно викликати саму себе, доки не буде досягнуто граничного розміру стеку, встановленого операційною системою. В цей момент програма аварійно завершиться з помилкою переповнення стеку (stack overflow).

## Код програми (C)
```
#include <stdio.h>

void recursiveFunction(int depth) {
    char buffer[1024]; // Виділяємо 1 КБ на стеку на кожному рівні рекурсії

    // Заповнюємо буфер, щоб компілятор не оптимізував змінну
    for (int i = 0; i < 1024; i++) {
        buffer[i] = i % 256;
    }

    printf("Recursion depth: %d\n", depth);

    // Рекурсивний виклик
    recursiveFunction(depth + 1);
}

int main() {
    recursiveFunction(1);
    return 0;
}
```
## Компіляція та запуск у FreeBSD
```
root@host:/home/guest/aspz_labs/lab3 # ./3-6
```

# По варіанту
Демонстрація обмеження динамічної пам'яті за допомогою ulimit -d
## Опис завдання
Ця програма демонструє, як команда ulimit -d використовується для обмеження обсягу динамічної пам'яті, яку процес може виділити. Програма намагається виділяти блоки пам'яті розміром 1 МБ у циклі, доки не досягне ліміту, встановленого ulimit -d. Після досягнення ліміту, виклик malloc поверне NULL, програма виведе повідомлення про помилку та завершить роботу.
Код програми (C)
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024 * 1024  // 1 МБ

int main() {
    int count = 0;
    while (1) {
        void *ptr = malloc(BLOCK_SIZE);
        if (ptr == NULL) {
            printf("Allocation failed at block #%d (approx %d MB)\n", count, count);
            break;
        }
// Заповнюємо пам'ять, щоб уникнути оптимізації
        memset(ptr, 0, BLOCK_SIZE);
        count++;
    }

    return 0;
}
```
## Компіляція та запуск у FreeBSD
root@host:/home/guest/aspz_labs/lab3 # ./variant
Досягнуто максимального розміру файлу (1024 байт).
