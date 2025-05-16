#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  // Для gettimeofday

// Функція для вимірюваного фрагмента (демо — затримка через цикл)
void test_code() {
    volatile long long sum = 0;
    for (long long i = 0; i < 100000000; ++i) {
        sum += i;
    }
}

int main() {
    struct timeval start, end;

    gettimeofday(&start, NULL);   // Початковий час

    test_code();                  // Фрагмент коду, який вимірюємо

    gettimeofday(&end, NULL);     // Кінцевий час

    // Розрахунок різниці в мілісекундах
    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;
    double elapsed_ms = seconds * 1000.0 + micros / 1000.0;

    printf("Час виконання: %.3f мс\n", elapsed_ms);
    return 0;
}
