# Практична робота 5
## Завдання 5-1 — Про помилки роботи з пам’яттю в Linux та засоби їх усунення

## Формулювання завдання

Побудуйте тест, де дві змінні випадково використовують ту саму адресу пам’яті (через обхід типів) — і виявити це можливо лише за допомогою логування.

## Опис виконання

Було створено буфер `uint8_t buffer[8]`, на який через приведення типів спрямовано два вказівники: `int *pi` та `float *pf`.
Змінні працюють з однією і тією ж областю пам’яті, проте інтерпретують її по-різному.
Програма виводить значення й адреси, що дозволяє побачити, що дві змінні працюють з тією самою адресою, але з різним типом.
Це приклад `aliasing` — ситуації, коли одна область пам’яті представлена через різні типи.
## Код: `5-1.c`

```
#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint8_t buffer[8];

    int *pi = (int*)(buffer);
    float *pf = (float*)(buffer);

    *pf = 13.37f;

    printf("=== Memory Aliasing Test ===\n");
    printf("Address of *pf: %p, value: %.2f\n", (void*)pf, *pf);
    printf("Address of *pi: %p, value: %d\n", (void*)pi, *pi);

    if ((void*)pf == (void*)pi) {
        printf("!!! Warning: pf and pi alias the same memory region!\n");
    }

    return 0;
}
```

## Компіляція та запуск

```
root@host:/home/guest/labs/lab5 # cc -Wall -o 5-1 5-1.c
root@host:/home/guest/labs/lab5 # ./5-1
=== Memory Aliasing Test ===
Address of *pf: 0x820f291e4, value: 13.37
Address of *pi: 0x820f291e4, value: 1096149893
!!! Warning: pf and pi alias the same memory region!
```

