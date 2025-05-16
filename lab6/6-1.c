#include <stdio.h>

int* get_ptr() {
    int x = 123;
    return &x; // Повертаємо адресу локальної змінної (це помилка!)
}

void clobber_stack() {
    int dummy[100]; // Забиваємо стек випадковими даними
    for (int i = 0; i < 100; i++) {
        dummy[i] = i;
    }
}

int main() {
    int *ptr = get_ptr();

    printf("Значення після повернення з функції: %d\n", *ptr);

    clobber_stack(); // Може перезаписати пам’ять, яку займала змінна x

    printf("Значення після перезапису стеку: %d\n", *ptr);

    return 0;
}
