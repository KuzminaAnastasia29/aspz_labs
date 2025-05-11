#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024 * 1024  // 1 МБ

int main() {
    int count = 0;
    while (1) {
        void *ptr = malloc(BLOCK_SIZE);
        if (ptr == NULL) {
            printf("Allocation failed at block #%d (approx %d MB)\n", count, count);
            break;
        }

        // Заповнюємо пам’ять, щоб уникнути оптимізації
        memset(ptr, 0, BLOCK_SIZE);
        count++;
    }

    return 0;
}
