#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Виконання команди whoami:\n");
    int whoami_status = system("whoami");

    if (whoami_status == -1) {
        perror("Помилка при виконанні whoami");
    }

    printf("\nВиконання команди id:\n");
    int id_status = system("id");

    if (id_status == -1) {
        perror("Помилка при виконанні id");
    }

    return 0;
}
