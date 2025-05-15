#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int main() {
    // Друкуємо розмір типу size_t
    printf("sizeof(size_t) = %zu байт\n", sizeof(size_t));
    
    // Друкуємо максимальне значення size_t
    size_t max = SIZE_MAX;
    printf("Максимальне значення size_t: %zu (0x%zx)\n", max, max);

    // Спроба виділити максимально можливу кількість пам’яті
    void* ptr = malloc(max);

    if (ptr == NULL) {
        printf("malloc(%zu) не вдалося: недостатньо пам’яті або перевищено ліміт\n", max);
    } else {
        printf("malloc(%zu) вдалося!\n", max);
        free(ptr);
    }

    return 0;
}
