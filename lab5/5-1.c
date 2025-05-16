#include <stdio.h>
#include <stdint.h>

int main(void) {
    // Масив байтів, який симулює спільну пам’ять
    uint8_t buffer[8];

    // Отримаємо два вказівники до buffer — через int* та через float*
    int *pi = (int*)(buffer);
    float *pf = (float*)(buffer);

    // Присвоюємо значення через float-вказівник
    *pf = 13.37f;

    // Тепер читаємо значення через int-вказівник
    printf("=== Memory Aliasing Test ===\n");
    printf("Address of *pf: %p, value: %.2f\n", (void*)pf, *pf);
    printf("Address of *pi: %p, value: %d\n", (void*)pi, *pi);

    // Якщо адреси однакові — це явне перекриття пам’яті
    if ((void*)pf == (void*)pi) {
        printf("!!! Warning: pf and pi alias the same memory region!\n");
    } else {
        printf("pf and pi are in different memory regions.\n");
    }

    return 0;
}
