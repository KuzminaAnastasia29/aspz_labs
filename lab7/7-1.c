#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *rwho_fp, *more_fp;
    char buffer[1024];

    // Відкриваємо команду rwho на читання
    rwho_fp = popen("rwho", "r");
    if (rwho_fp == NULL) {
        perror("Не вдалося запустити rwho");
        exit(EXIT_FAILURE);
    }

    // Відкриваємо more на запис
    more_fp = popen("more", "w");
    if (more_fp == NULL) {
        perror("Не вдалося запустити more");
        pclose(rwho_fp);
        exit(EXIT_FAILURE);
    }

    // Передаємо дані з rwho до more
    while (fgets(buffer, sizeof(buffer), rwho_fp) != NULL) {
        fputs(buffer, more_fp);
    }

    // Закриваємо потоки
    pclose(rwho_fp);
    pclose(more_fp);

    return 0;
}
