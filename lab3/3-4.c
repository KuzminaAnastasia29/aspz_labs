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
