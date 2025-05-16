#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char response[4];

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Пропустити "." і ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Отримати інформацію про файл
        if (stat(entry->d_name, &st) == -1) {
            perror("stat");
            continue;
        }

        // Працюємо лише зі звичайними файлами
        if (S_ISREG(st.st_mode)) {
            printf("Файл: %s — Видалити? (y/n): ", entry->d_name);
            fgets(response, sizeof(response), stdin);

            if (response[0] == 'y' || response[0] == 'Y') {
                if (remove(entry->d_name) == 0) {
                    printf("Видалено \n");
                } else {
                    perror("remove");
                }
            }
        }
    }

    closedir(dir);
    return 0;
}
