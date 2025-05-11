#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#define FILENAME "dice_rolls.txt"
#define MAX_FILE_SIZE 1024  // Обмеження у байтах (наприклад, 1 КБ)

int get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return 0;
}

int main() {
    FILE *file;
    int roll;
    int file_size;

    // Ініціалізація генератора випадкових чисел
    srand(time(NULL));

    // Відкриття файлу для дозапису
    file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Не вдалося відкрити файл");
        return 1;
    }

    while (1) {
        file_size = get_file_size(FILENAME);
        if (file_size >= MAX_FILE_SIZE) {
            printf("Досягнуто максимального розміру файлу (%d байт).\n", MAX_FILE_SIZE);
            break;
        }

        // Імітація кидка кубика (від 1 до 6)
        roll = rand() % 6 + 1;

        // Запис результату у файл
        fprintf(file, "%d\n", roll);
    }

    fclose(file);
    return 0;
}
