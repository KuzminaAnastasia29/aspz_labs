# Практична робота 8
## Завдання 8.1
## Формулювання завдання
Чи може виклик `count = write(fd, buffer, nbytes)`; повернути в змінній count значення, відмінне від `nbytes`? Якщо так, то чому? Наведіть робочий приклад програми, яка демонструє вашу відповідь.

## Відповідь
Так, виклик `write()` може повернути значення, відмінне від `nbytes`.
Це означає, що було записано меншу кількість байтів, ніж запитано. Причини можуть бути різними:

Буфер пристрою або файловий дескриптор не може прийняти весь обсяг даних одразу (наприклад, файл відкритий у неблокуючому режимі).

Пам'ять або ресурси пристрою тимчасово обмежені.

Запис прервався через сигнал або іншу системну подію.

Тому завжди потрібно перевіряти повернене значення `write` і, за необхідності, робити повторні виклики, доки не буде записано всі байти.
## Опис виконання

Було написано тестову програму, яка намагається записати в `stdout` (консоль) великий буфер 1МБ. В результаті фактичний запис у один виклик `write()` обмежився розміром буфера системного кеша (наприклад, 64 КБ), тому `write() `повернув менше ніж `nbytes`.

Цей приклад ілюструє, що `write()` не гарантує запис всіх байтів за один виклик.


## Код: `8-1.c`

```
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ONE_MB 1024*1024

int main() {
    char buffer[ONE_MB];
    memset(buffer, 'A', ONE_MB);

    ssize_t count = write(STDOUT_FILENO, buffer, ONE_MB);
    printf("\nRequested %d bytes, actually wrote %zd bytes\n", ONE_MB, count);

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab8 # cc -Wall -o 8-1 8-1.c
root@host:/home/guest/labs/lab8 # ./8-1
Requested 1MB, actually wrote 65536 bytes
```
## Завдання 8.2
## Формулювання завдання
Є файл, дескриптор якого — fd. Файл містить таку послідовність байтів: 4, 5, 2, 2, 3, 3, 7, 9, 1, 5. У програмі виконується наступна послідовність системних викликів:
```
lseek(fd, 3, SEEK_SET);
read(fd, &buffer, 4);
``` 
де виклик lseek переміщує покажчик на третій байт файлу. Що буде містити буфер після завершення виклику read? Наведіть робочий приклад програми, яка демонструє вашу відповідь.

## Відповідь
Файл містить послідовність байтів:
[4][5][2][2][3][3][7][9][1][5]

Індексація байтів (починаючи з 0):

```
0: 4
1: 5
2: 2
3: 2  <-- lseek ставить курсор сюди
4: 3
5: 3
6: 7
7: 9
8: 1
9: 5
```
Виклик `lseek(fd, 3, SEEK_SET)` встановлює позицію для читання на 4-й байт (індекс 3).
Потім виклик `read(fd, &buffer, 4)` прочитає 4 байти, починаючи з позиції 3:
Це байти з індексами 3,4,5,6 → 2, 3, 3, 7.

Отже, buffer після виклику read міститиме саме цю послідовність.
## Опис виконання

У програмі відкривається тимчасовий файл, у який записується задана послідовність байтів. Потім виконується `lseek` і read відповідно до умови.

## Код: `8-2.c`

```
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    const char data[] = {4,5,2,2,3,3,7,9,1,5};
    char buffer[4];
    int fd;

    fd = open("temp.bin", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("write");
        close(fd);
        return 1;
    }

    // Встановити позицію на 3-й байт
    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    // Прочитати 4 байти
    ssize_t n = read(fd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("Прочитано %zd байт: ", n);
    for (int i = 0; i < n; i++)
        printf("%d ", (unsigned char)buffer[i]);
    printf("\n");

    close(fd);
    unlink("temp.bin"); // Видалити тимчасовий файл

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab8 # cc -Wall -o 8-2 8-2.c
root@host:/home/guest/labs/lab8 # ./8-2
Read bytes: 2 3 3 7 
```

## Завдання 8.3
## Формулювання завдання
Бібліотечна функція qsort призначена для сортування даних будь-якого типу. Для її роботи необхідно підготувати функцію порівняння, яка викликається з qsort кожного разу, коли потрібно порівняти два значення.
 Оскільки значення можуть мати будь-який тип, у функцію порівняння передаються два вказівники типу void* на елементи, що порівнюються.
Напишіть програму, яка досліджує, які вхідні дані є найгіршими для алгоритму швидкого сортування. Спробуйте знайти кілька масивів даних, які змушують qsort працювати якнайповільніше. Автоматизуйте процес експериментування так, щоб підбір і аналіз вхідних даних виконувалися самостійно.

Придумайте і реалізуйте набір тестів для перевірки правильності функції `qsort`.

## Відповідь
Що таке "найгірші вхідні дані" для `qsort`?
Хоча реалізація qsort залежить від компілятора та платформи, типові найгірші випадки для класичного `quicksort`:

Вже відсортований масив (за зростанням або спаданням).

Масив з великою кількістю однакових елементів.

Масив, який має патологічну структуру для вибору опорного елемента (`pivot`).

Ці випадки можуть призводити до деградації складності з O(n log n) до O(n²).
## Опис виконання

Програма виконує такі дії:

Вимірює час сортування різних варіантів масивів (відсортований, зворотний, однакові елементи, випадковий).

Для тестів правильності сортування генерує масиви і перевіряє, чи відсортовані вони після `qsort`.


## Код: `8-3test.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int is_sorted(int *arr, int n) {
    for (int i = 1; i < n; ++i)
        if (arr[i - 1] > arr[i])
            return 0;
    return 1;
}

void test_qsort_correctness() {
    int test1[] = {5, 4, 3, 2, 1};
    int test2[] = {1, 1, 1, 1, 1};
    int test3[] = {2, 3, 1, 4, 5};
    int test4[] = {};
    int test5[] = {1};

    int *tests[] = {test1, test2, test3, test4, test5};
    int sizes[] = {5, 5, 5, 0, 1};

    for (int i = 0; i < 5; ++i) {
        qsort(tests[i], sizes[i], sizeof(int), cmp_int);
        if (!is_sorted(tests[i], sizes[i]))
            printf(" Тест %d не пройдено\n", i + 1);
        else
            printf(" Тест %d пройдено\n", i + 1);
    }
}

void fill_array(int *arr, int n, int type) {
    if (type == 0) { // зворотній порядок
        for (int i = 0; i < n; ++i)
            arr[i] = n - i;
    } else if (type == 1) { // всі однакові
        for (int i = 0; i < n; ++i)
            arr[i] = 42;
    } else if (type == 2) { // випадкові
        for (int i = 0; i < n; ++i)
            arr[i] = rand() % 10000;
    } else if (type == 3) { // вже відсортовані
        for (int i = 0; i < n; ++i)
            arr[i] = i;
    }
}

void benchmark(int size, int type, const char *label) {
    int *arr = malloc(size * sizeof(int));
    fill_array(arr, size, type);

    clock_t start = clock();
    qsort(arr, size, sizeof(int), cmp_int);
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%s (n=%d): %.6f сек\n", label, size, elapsed);

    free(arr);
}

int main() {
    srand(time(NULL));

    printf("== Тести правильності ==\n");
    test_qsort_correctness();

    printf("\n== Бенчмарки (повільні варіанти qsort) ==\n");
    benchmark(100000, 0, "Зворотній порядок");
    benchmark(100000, 1, "Всі однакові");
    benchmark(100000, 2, "Випадкові значення");
    benchmark(100000, 3, "Вже відсортовані");

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab8 # cc -Wall -o 8-3 8-3.c
root@host:/home/guest/labs/lab8 # ./8-3
Array size: 1000
  Sorted:     0.000000 sec
  Reversed:   0.000000 sec
  All equal:  0.000000 sec
  Random:     0.000000 sec

Array size: 5000
  Sorted:     0.000000 sec
  Reversed:   0.000000 sec
  All equal:  0.000000 sec
  Random:     0.000000 sec

Array size: 10000
  Sorted:     0.000000 sec
  Reversed:   0.000000 sec
  All equal:  0.000000 sec
  Random:     0.000000 sec

Array size: 50000
  Sorted:     0.007812 sec
  Reversed:   0.000000 sec
  All equal:  0.000000 sec
  Random:     0.015625 sec

Array size: 100000
  Sorted:     0.007812 sec
  Reversed:   0.000000 sec
  All equal:  0.000000 sec
  Random:     0.015625 sec
```

## Завдання 8.4
## Формулювання завдання

 Виконайте наступну програму на мові програмування С:
int main() {
  int pid;
  pid = fork();
  printf("%d\n", pid);
}
Завершіть цю програму. Припускаючи, що виклик `fork()` був успішним, яким може бути результат виконання цієї програми?
## Відповідь
Системний виклик `fork()` створює новий процес. Після його виклику виконання продовжується одночасно у двох процесах:

У батьківському процесі `fork()` повертає PID дочірнього процесу (додатне число),

У дочірньому — 0.

У моєму результаті:
```

pid = 1635, process ID = 1634
pid = 0, process ID = 1635
```
це означає:

Процес з PID = 1634 — батько, отримав від `fork()` значення 1635 (тобто PID створеного дочірнього процесу).

Процес з PID = 1635 — дитина, отримала від fork() значення 0.

Цей результат повністю відповідає очікуваній поведінці `fork()`


## Код: `8-4.c`

```
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    pid = fork();

    if (pid < 0) {
        // Помилка створення процесу
        perror("fork failed");
        return 1;
    }

    printf("pid = %d, process ID = %d\n", pid, getpid());
    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab8 # cc -Wall -o 8-4 8-4.c
root@host:/home/guest/labs/lab8 # ./8-4
pid = 1134, process ID = 1133
pid = 0, process ID = 1134
```

## Завдання 8.variant9
## Формулювання завдання
Проведіть тестування: що буде, якщо виконати `fork()` усередині циклу for із затримкою між викликами?
## Опис виконання

На кожній ітерації кожен активний процес викликає `fork()`, створюючи нового нащадка.

Через `sleep(1)` між ітераціями можна чітко бачити, як кількість процесів зростає.

Кожен новий процес продовжує виконання циклу з того самого місця.

## Код: `8-variant9.c`

```
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    for (int i = 0; i < 3; ++i) {
        printf("Iteration %d, PID: %d, Parent PID: %d\n", i, getpid(), getppid());
        fork();
        sleep(1);
    }
    return 0;
} 
```

## Компіляція

```
root@host:/home/guest/labs/lab8 # cc -Wall -o 8-variant9 8-variant9.c
root@host:/home/guest/labs/lab8 # ./8-variant9
Iteration 0, PID: 1140, Parent PID: 958
Iteration 1, PID: 1141, Parent PID: 1140
Iteration 1, PID: 1140, Parent PID: 958
Iteration 2, PID: 1141, Parent PID: 1140
Iteration 2, PID: 1143, Parent PID: 1140
Iteration 2, PID: 1142, Parent PID: 1141
Iteration 2, PID: 1140, Parent PID: 958
```


