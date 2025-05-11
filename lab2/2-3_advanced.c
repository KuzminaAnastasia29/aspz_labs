#include <stdio.h>
#include <stdlib.h>

int global_init = 42;               // .data (ініціалізовані глобальні)
int global_uninit;                  // .bss (неініціалізовані глобальні)

void dummy_function() {}           // розміщується в .text

int main() {
    int local_var = 1;             // стек
    static int static_var = 2;     // .data
    static int static_uninit;      // .bss

    int *heap_var = malloc(sizeof(int) * 10);  // купа (heap)

    printf("Code (.text)        is near: %p\n", (void*)&dummy_function);
    printf("Init data (.data)   is near: %p\n", (void*)&global_init);
    printf("Uninit (.bss)       is near: %p\n", (void*)&global_uninit);
    printf("Static (.data)      is near: %p\n", (void*)&static_var);
    printf("Static uninit (.bss) is near: %p\n", (void*)&static_uninit);
    printf("Heap                is near: %p\n", (void*)heap_var);
    printf("Stack (local var)   is near: %p\n", (void*)&local_var);

    free(heap_var);
    return 0;
}
