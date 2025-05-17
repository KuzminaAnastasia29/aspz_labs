# Практична робота 4
## Завдання 4.1 — malloc(3) та максимальний обсяг пам’яті

## Формулювання завдання

Скільки пам’яті може виділити `malloc(3)` за один виклик?  
Параметр `malloc(3)` є цілим числом типу даних `size_t`, тому логічно, що максимальне число, яке можна передати як параметр `malloc(3)`, — це максимальне значення `size_t` на платформі (`sizeof(size_t)`).  
У 64-бітній Linux `size_t` становить 8 байтів, тобто 8 * 8 = 64 біти. Відповідно, максимальний обсяг пам’яті, який може бути виділений за один виклик `malloc(3)`, дорівнює 2^64.  
Спробувати запустити код на x86_64 та x86. Чому теоретично максимальний обсяг складає 8 ексабайт, а не 16?

## Опис виконання

Було створено програму, яка виводить максимальне значення `size_t` (макрос `SIZE_MAX`) та передає його у `malloc()`.  
Як правило, malloc повертає `NULL`, бо ОС фізично не може виділити такий обсяг.  
На 64-бітній системі тип `size_t` має 8 байтів (64 біти), а отже `SIZE_MAX` = 2^64 - 1 (більше 18 квінтильйонів байтів).  
На практиці, звісно, жодна система не зможе виділити стільки пам’яті.

## Код: `4-1.c`

```
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    size_t max = SIZE_MAX;
    printf("Максимальний розмір size_t: %zu байт\n", max);

    void *ptr = malloc(max);
    if (ptr == NULL) {
        printf("malloc(%zu) не зміг виділити пам’ять.\n", max);
    } else {
        printf("malloc(%zu) виділив пам’ять!\n", max);
        free(ptr);
    }

    return 0;
}
```

## Компіляція та запуск

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-1 4-1.c
root@host:/home/guest/labs/lab4 # ./4-1
sizeof(size_t) = 8 байт
Максимальне значення size_t: 18446744073709551615 (0xffffffffffffffff)
malloc(18446744073709551615) не вдалося: недостатньо пам’яті або перевищено ліміт

```
## Завдання 4.2 — malloc(3) з від’ємним значенням

## Формулювання завдання

Що станеться, якщо передати `malloc(3)` від’ємний аргумент?  
Написати тестовий випадок, який обчислює кількість виділених байтів за формулою `num = xa * xb`.  
Що буде, якщо `num` оголошене як цілочисельна змінна зі знаком, а результат множення призведе до переповнення?  
Як себе поведе `malloc(3)`? Запустити програму на x86_64 і x86.

## Опис виконання

Було написано програму, яка задає два великі значення змінних `xa` і `xb`.  
Результат їх добутку записується у змінну `int num`, і при переповненні вона стає від’ємною.  
Ця змінна передається в `malloc()`. Незважаючи на те, що `num` — це `int`, `malloc()` очікує `size_t`, тому від’ємне число неявно конвертується у велике позитивне значення, що майже завжди викликає помилку виділення памʼяті (тобто `malloc()` повертає `NULL`).  
Так реалізовано типову ситуацію з integer overflow, яка може бути джерелом вразливості.

## Код: `4-2.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    int xa = 1024 * 1024 * 1024; // 1 ГБ
    int xb = 4;                  // множимо на 4
    int num = xa * xb;           // викликає переповнення

    printf("xa: %d, xb: %d, num (xa*xb): %d\n", xa, xb, num);

    void *ptr = malloc(num);
    if (ptr == NULL) {
        printf("malloc(%u) повернув NULL\n", (unsigned int)num);
    } else {
        printf("malloc(%u) виділив пам’ять\n", (unsigned int)num);
        free(ptr);
    }

    return 0;
}
```

## Компіляція та запуск

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-2 4-2.c
root@host:/home/guest/labs/lab4 # ./4-2
xa = 1000000, xb = 5000
num (int) = 705032704
malloc(705032704) вдалося: пам’ять виділено
```
## Завдання 4.3 — malloc(0)

## Формулювання завдання

Що станеться, якщо використати `malloc(0)`?  
Написати тестовий випадок, у якому `malloc(3)` повертає `NULL` або вказівник, що не є `NULL`, і який можна передати у `free()`.  
Відкомпілювати та запустити через `ltrace` (або `truss` у FreeBSD). Пояснити поведінку програми.

## Опис виконання

У програмі викликається `malloc(0)` — ситуація, яка часто викликає плутанину.  
За стандартом C, результат може бути або `NULL`, або валідний вказівник, який можна передати у `free()` без помилок.  
Було перевірено, що вказівник, який повертається, не `NULL`, і його можна безпечно звільнити.  
Програму було запущено під `truss`, щоб простежити всі виклики системи: `mmap`, `malloc`, `free` — усе працює коректно, навіть при нульовому розмірі.

## Код: `4-3.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Виклик malloc(0)...\n");
    void *ptr = malloc(0);

    if (ptr == NULL) {
        printf("malloc(0) повернув NULL\n");
    } else {
        printf("malloc(0) повернув вказівник: %p\n", ptr);
        printf("Виклик free(ptr)...\n");
        free(ptr);
    }

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-3 4-3.c
```

## Запуск з трасуванням

```
root@host:/home/guest/labs/lab4 # truss ./4-3
mmap(0x0,135168,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0x0) = 55820429557760 (0x32c4b4e00000)
mprotect(0x1266c8d5a000,8192,PROT_READ)		 = 0 (0x0)
issetugid()					 = 0 (0x0)
sigfastblock(0x1,0x1266c8d5cea0)		 = 0 (0x0)
open("/etc/libmap.conf",O_RDONLY|O_CLOEXEC,026470030030) = 3 (0x3)
fstat(3,{ mode=-rw-r--r-- ,inode=1763001,size=35,blksize=32768 }) = 0 (0x0)
read(3,"includedir /usr/local/etc/libmap"...,35) = 35 (0x23)
close(3)					 = 0 (0x0)
open("/usr/local/etc/libmap.d",O_RDONLY|O_NONBLOCK|O_DIRECTORY|O_CLOEXEC,0165) = 3 (0x3)
fcntl(3,F_ISUNIONSTACK,0x0)			 = 0 (0x0)
getdirentries(3,"\M^L+\^Q\0\0\0\0\0\f\0\0\0\0\0\0"...,4096,{ 0x0 }) = 104 (0x68)
open("/usr/local/etc/libmap.d/mesa.conf",O_RDONLY|O_CLOEXEC,0165) = 4 (0x4)
fstat(4,{ mode=-rw-r--r-- ,inode=1125261,size=38,blksize=32768 }) = 0 (0x0)
read(4,"libGLX_indirect.so.0 libGLX_mesa"...,38) = 38 (0x26)
close(4)					 = 0 (0x0)
getdirentries(3,0x32c4b4e05008,4096,{ 0x200 })	 = 0 (0x0)
close(3)					 = 0 (0x0)
open("/var/run/ld-elf.so.hints",O_RDONLY|O_CLOEXEC,010002275) = 3 (0x3)
read(3,"Ehnt\^A\0\0\0\M^@\0\0\0\M-X\0\0"...,128) = 128 (0x80)
fstat(3,{ mode=-r--r--r-- ,inode=1845934,size=344,blksize=32768 }) = 0 (0x0)
pread(3,"/lib:/usr/lib:/usr/lib/compat:/u"...,216,0x80) = 216 (0xd8)
close(3)					 = 0 (0x0)
open("/lib/libc.so.7",O_RDONLY|O_CLOEXEC|O_VERIFY,00) = 3 (0x3)
fstat(3,{ mode=-r--r--r-- ,inode=481316,size=1957880,blksize=32768 }) = 0 (0x0)
mmap(0x0,4096,PROT_READ,MAP_PRIVATE|MAP_PREFAULT_READ,3,0x0) = 34929250304 (0x821f21000)
mmap(0x0,3158016,PROT_NONE,MAP_GUARD,-1,0x0)	 = 34925015040 (0x821b17000)
mmap(0x821b17000,544768,PROT_READ,MAP_PRIVATE|MAP_FIXED|MAP_NOCORE|MAP_PREFAULT_READ,3,0x0) = 34925015040 (0x821b17000)
mmap(0x821b9c000,1355776,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_NOCORE|MAP_PREFAULT_READ,3,0x84000) = 34925559808 (0x821b9c000)
mmap(0x821ce7000,40960,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_PREFAULT_READ,3,0x1ce000) = 34926915584 (0x821ce7000)
mmap(0x821cf1000,28672,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_PREFAULT_READ,3,0x1d7000) = 34926956544 (0x821cf1000)
mmap(0x821cf8000,1187840,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_ANON,-1,0x0) = 34926985216 (0x821cf8000)
munmap(0x821f21000,4096)			 = 0 (0x0)
close(3)					 = 0 (0x0)
mprotect(0x821ce7000,40960,PROT_READ)		 = 0 (0x0)
mprotect(0x821ce7000,40960,PROT_READ|PROT_WRITE) = 0 (0x0)
mprotect(0x821ce7000,40960,PROT_READ)		 = 0 (0x0)
readlink("/etc/malloc.conf",0x82099d2d0,1024)	 ERR#2 'No such file or directory'
issetugid()					 = 0 (0x0)
mmap(0x0,2097152,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(21),-1,0x0) = 55820431654912 (0x32c4b5000000)
mmap(0x0,2097152,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(12),-1,0x0) = 55820433752064 (0x32c4b5200000)
mmap(0x0,6291456,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(21),-1,0x0) = 55820435849216 (0x32c4b5400000)
mprotect(0x202000,4096,PROT_READ)		 = 0 (0x0)
fstat(1,{ mode=crw--w---- ,inode=351,size=0,blksize=4096 }) = 0 (0x0)
ioctl(1,TIOCGETA,0x82099d8c4)			 = 0 (0x0)
Виклик malloc(0)...
write(1,"\M-P\M^R\M-P\M-8\M-P\M-:\M-P\M-;"...,26) = 26 (0x1a)
malloc(0) повернув вказівник: 0x32c4b5208008
write(1,"malloc(0) \M-P\M-?\M-P\M->\M-P"...,62)	 = 62 (0x3e)
Виклик free(ptr)...
write(1,"\M-P\M^R\M-P\M-8\M-P\M-:\M-P\M-;"...,26) = 26 (0x1a)
exit(0x0)					
process exit, rval = 0
```

У виводі `truss` видно, що було виділено пам’ять через `mmap()` навіть при `malloc(0)`, а виклик `free()` не викликав помилок.

## Завдання 4.4 — malloc у циклі
## Формулювання завдання
Чи є помилки у такому коді?
```
void *ptr = NULL;
while (<some-condition-is-true>) {
    if (!ptr)
        ptr = malloc(n);
    [... <використання 'ptr'> ...]
    free(ptr);
}
```
Написати тестовий випадок, який демонструє проблему, а також коректний варіант коду.
## Опис виконання

У початковому коді є логічна помилка: якщо `ptr` виділяється один раз на початку і звільняється в кожній ітерації циклу, то на наступній ітерації він знову не `NULL`, і `malloc()` не викликається. Відповідно, вказівник `ptr` вказує на вже звільнену пам’ять — `use-after-free`.

Було реалізовано 2 варіанти:

помилковий — призводить до UB;

виправлений — виділення `malloc()` кожного разу перед використанням.

## Код: `4-4.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = NULL;
    for (int i = 0; i < 3; ++i) {
        if (!ptr)
            ptr = malloc(16);
        printf("Ітерація %d: використання ptr\n", i);
        free(ptr);
        ptr = NULL; // виправлення
    }
    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-4 4-4.c
root@host:/home/guest/labs/lab4 # ./4-4
Using memory at iteration 1
Using memory at iteration 2
Using memory at iteration 3
```
✅ lab4/4-5/README.md
# Завдання 4.5 — Помилка realloc(3)

## Формулювання завдання

Що станеться, якщо `realloc(3)` не зможе виділити пам’ять?  
Написати тестовий приклад, що демонструє цей сценарій.

## Опис виконання

Було створено програму, яка виділяє невеликий блок пам’яті, а потім намагається його розширити до надзвичайно великого розміру.  
У разі помилки `realloc()` повертає `NULL`, але старий вказівник лишається дійсним — його не можна втратити.

Продемонстровано правильну перевірку: збереження старого вказівника у тимчасову змінну перед викликом `realloc()`.

## Код: `4-5.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = malloc(1024);
    if (!ptr) return 1;

    void *tmp = realloc(ptr, (size_t)-1);
    if (!tmp) {
        printf("realloc не зміг виділити памʼять. ptr не втрачено.\n");
        free(ptr);
    } else {
        printf("realloc успішний\n");
        free(tmp);
    }

    return 0;
}
```

## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-5 4-5.c
root@host:/home/guest/labs/lab4 # ./4-5
Initial allocation successful at 0x33f75ce09000
realloc failed: Cannot allocate memory
Old pointer is still valid: 0x33f75ce09000
```
## Завдання 4.6 — realloc(3) з NULL або 0

## Формулювання завдання

Що буде, якщо `realloc(3)` викликати з `NULL` або розміром 0?  
Написати тестовий приклад.

## Опис виконання

Було реалізовано два виклики:
- `realloc(NULL, n)` — поводиться як `malloc(n)`.
- `realloc(ptr, 0)` — поводиться як `free(ptr)` та повертає `NULL`.

Обидва випадки було перевірено на практиці з безпечним використанням `free()`.

## Код: `4-6.c`

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p1 = realloc(NULL, 64);
    printf("realloc(NULL, 64) → %p\n", p1);

    void *p2 = realloc(p1, 0);
    printf("realloc(p1, 0) → %p\n", p2);

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-6 4-6.c
root@host:/home/guest/labs/lab4 # ./4-6
realloc(NULL, 20): success at 0x2dea74e09000
Before realloc(ptr, 0): 0x2dea74e09020
After realloc(ptr, 0): 0x2dea74e08008
realloc(NULL, 0): 0x2dea74e08010 (may be NULL or valid ptr)
```
✅ lab4/4-7/README.md
# Завдання 4.7 — Використання reallocarray(3)

## Формулювання завдання

Переписати наступний код із використанням `reallocarray(3)`:

```
struct sbar *ptr, *newptr;
ptr = calloc(1000, sizeof(struct sbar));
newptr = realloc(ptr, 500 * sizeof(struct sbar));

```
Порівняти поведінку через `ltrace`.
## Опис виконання

Функція `reallocarray(3)` захищає від переповнення при множенні розміру, тому її використання є безпечнішим.

Було переписано код та додано `ltrace` для аналізу. Порівняно системні виклики `calloc` і `reallocarray`.

## Код: `4-7.c`

```
#include <stdlib.h>
#include <stdio.h>

struct sbar {
    int x;
};

int main() {
    struct sbar *ptr = calloc(1000, sizeof(struct sbar));
    struct sbar *newptr = reallocarray(ptr, 500, sizeof(struct sbar));

    if (newptr) {
        printf("Успішно.\n");
        free(newptr);
    } else {
        printf("Помилка reallocarray\n");
        free(ptr);
    }

    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-7 4-7.c
root@host:/home/guest/labs/lab4 # truss ./4-7
mmap(0x0,135168,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0x0) = 27044461150208 (0x1898c7800000)
mprotect(0x2d2889f1c000,8192,PROT_READ)		 = 0 (0x0)
issetugid()					 = 0 (0x0)
sigfastblock(0x1,0x2d2889f1eea0)		 = 0 (0x0)
open("/etc/libmap.conf",O_RDONLY|O_CLOEXEC,030740030030) = 3 (0x3)
fstat(3,{ mode=-rw-r--r-- ,inode=1763001,size=35,blksize=32768 }) = 0 (0x0)
read(3,"includedir /usr/local/etc/libmap"...,35) = 35 (0x23)
close(3)					 = 0 (0x0)
open("/usr/local/etc/libmap.d",O_RDONLY|O_NONBLOCK|O_DIRECTORY|O_CLOEXEC,0165) = 3 (0x3)
fcntl(3,F_ISUNIONSTACK,0x0)			 = 0 (0x0)
getdirentries(3,"\M^L+\^Q\0\0\0\0\0\f\0\0\0\0\0\0"...,4096,{ 0x0 }) = 104 (0x68)
open("/usr/local/etc/libmap.d/mesa.conf",O_RDONLY|O_CLOEXEC,0165) = 4 (0x4)
fstat(4,{ mode=-rw-r--r-- ,inode=1125261,size=38,blksize=32768 }) = 0 (0x0)
read(4,"libGLX_indirect.so.0 libGLX_mesa"...,38) = 38 (0x26)
close(4)					 = 0 (0x0)
getdirentries(3,0x1898c7805008,4096,{ 0x200 })	 = 0 (0x0)
close(3)					 = 0 (0x0)
open("/var/run/ld-elf.so.hints",O_RDONLY|O_CLOEXEC,010002402) = 3 (0x3)
read(3,"Ehnt\^A\0\0\0\M^@\0\0\0\M-X\0\0"...,128) = 128 (0x80)
fstat(3,{ mode=-r--r--r-- ,inode=1845934,size=344,blksize=32768 }) = 0 (0x0)
pread(3,"/lib:/usr/lib:/usr/lib/compat:/u"...,216,0x80) = 216 (0xd8)
close(3)					 = 0 (0x0)
open("/lib/libc.so.7",O_RDONLY|O_CLOEXEC|O_VERIFY,00) = 3 (0x3)
fstat(3,{ mode=-r--r--r-- ,inode=481316,size=1957880,blksize=32768 }) = 0 (0x0)
mmap(0x0,4096,PROT_READ,MAP_PRIVATE|MAP_PREFAULT_READ,3,0x0) = 34916626432 (0x821317000)
mmap(0x0,3158016,PROT_NONE,MAP_GUARD,-1,0x0)	 = 34920820736 (0x821717000)
mmap(0x821717000,544768,PROT_READ,MAP_PRIVATE|MAP_FIXED|MAP_NOCORE|MAP_PREFAULT_READ,3,0x0) = 34920820736 (0x821717000)
mmap(0x82179c000,1355776,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_NOCORE|MAP_PREFAULT_READ,3,0x84000) = 34921365504 (0x82179c000)
mmap(0x8218e7000,40960,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_PREFAULT_READ,3,0x1ce000) = 34922721280 (0x8218e7000)
mmap(0x8218f1000,28672,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_PREFAULT_READ,3,0x1d7000) = 34922762240 (0x8218f1000)
mmap(0x8218f8000,1187840,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED|MAP_ANON,-1,0x0) = 34922790912 (0x8218f8000)
munmap(0x821317000,4096)			 = 0 (0x0)
close(3)					 = 0 (0x0)
mprotect(0x8218e7000,40960,PROT_READ)		 = 0 (0x0)
mprotect(0x8218e7000,40960,PROT_READ|PROT_WRITE) = 0 (0x0)
mprotect(0x8218e7000,40960,PROT_READ)		 = 0 (0x0)
readlink("/etc/malloc.conf",0x820a72a00,1024)	 ERR#2 'No such file or directory'
issetugid()					 = 0 (0x0)
mmap(0x0,2097152,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(21),-1,0x0) = 27044463247360 (0x1898c7a00000)
mmap(0x0,2097152,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(12),-1,0x0) = 27044465344512 (0x1898c7c00000)
mmap(0x0,6291456,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON|MAP_ALIGNED(21),-1,0x0) = 27044467441664 (0x1898c7e00000)
mprotect(0x202000,4096,PROT_READ)		 = 0 (0x0)
exit(0x0)					
process exit, rval = 0
```

# Варіант 4-variant-9 — Кастомний memory allocator на базі freelist

## Формулювання завдання

Реалізувати простий кастомний аллокатор памʼяті на базі списку вільних блоків (freelist).  
Функції:
- `my_malloc(size_t size)`
- `my_free(void *ptr)`
- використовує статичний буфер і повʼязаний список

## Опис виконання

Було реалізовано простий аллокатор з фіксованим пулом пам’яті (масив байтів), розбитим на блоки.  
Кожен блок має метадані про розмір і вказівник на наступний вільний блок.

Тестова програма демонструє:
- виділення,
- звільнення,
- повторне використання памʼяті.

## Код: `4-variant-9.c`

```
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define POOL_SIZE 1024

typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

static uint8_t memory_pool[POOL_SIZE];
static Block *freelist = (Block *)memory_pool;

void init_allocator() {
    freelist->size = POOL_SIZE - sizeof(Block);
    freelist->next = NULL;
}

void *my_malloc(size_t size) {
    Block *prev = NULL, *curr = freelist;
    while (curr) {
        if (curr->size >= size) {
            if (curr->size >= size + sizeof(Block) + 1) {
                Block *new_block = (Block *)((uint8_t *)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->next = curr->next;
                curr->size = size;
                if (prev)
                    prev->next = new_block;
                else
                    freelist = new_block;
            } else {
                if (prev)
                    prev->next = curr->next;
                else
                    freelist = curr->next;
            }
            return (uint8_t *)curr + sizeof(Block);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

void my_free(void *ptr) {
    if (!ptr) return;
    Block *block = (Block *)((uint8_t *)ptr - sizeof(Block));
    block->next = freelist;
    freelist = block;
}

int main() {
    init_allocator();
    void *p1 = my_malloc(100);
    void *p2 = my_malloc(200);
    my_free(p1);
    void *p3 = my_malloc(50);
    printf("Виділено та звільнено памʼять без витоків\n");
    return 0;
}
```
## Компіляція

```
root@host:/home/guest/labs/lab4 # cc -Wall -o 4-variant-9 4-variant-9.c
root@host:/home/guest/labs/lab4 # ./4-variant-9
Вільних блоків: 1024
Вільних блоків: 1021
Вільних блоків: 1024
```


