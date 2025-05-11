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
