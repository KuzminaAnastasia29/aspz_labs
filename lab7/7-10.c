#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Генерує випадкове число від 0.0 до 1.0
double rand_0_1() {
    return (double)rand() / RAND_MAX;
}

// Генерує випадкове число від 0.0 до n (n > 0)
double rand_0_n(double n) {
    return rand_0_1() * n;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <верхня_межа_діапазону>\n", argv[0]);
        fprintf(stderr, "Якщо верхня межа = 1.0, генерує числа від 0.0 до 1.0\n");
        return 1;
    }

    double n = atof(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Помилка: верхня межа має бути додатним числом\n");
        return 1;
    }

    // Ініціалізація генератора випадкових чисел унікальним початковим значенням
    srand((unsigned int)time(NULL));

    printf("Генерація 10 випадкових чисел у діапазоні від 0.0 до %.3f:\n", n);

    for (int i = 0; i < 10; i++) {
        double r;
        if (n == 1.0) {
            r = rand_0_1();
        } else {
            r = rand_0_n(n);
        }
        printf("%f\n", r);
    }

    return 0;
}
