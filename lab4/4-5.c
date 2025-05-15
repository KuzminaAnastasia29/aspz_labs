#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t initial_size = 10;
    void *ptr = malloc(initial_size);

    if (!ptr) {
        perror("Initial malloc failed");
        return 1;
    }

    printf("Initial allocation successful at %p\n", ptr);

    // Намагатимемось виділити дуже багато пам’яті
    size_t huge_size = (size_t)-1; // найбільше можливе значення

    void *tmp = realloc(ptr, huge_size);
    if (!tmp) {
        perror("realloc failed");
        printf("Old pointer is still valid: %p\n", ptr);
        free(ptr); // звільняємо стару область вручну
    } else {
        printf("Realloc successful at %p\n", tmp);
        free(tmp);
    }

    return 0;
}
