#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int is_executable_by_user(const struct stat *st) {
    return (st->st_mode & S_IXUSR);
}

int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strcmp(str + lenstr - lensuffix, suffix) == 0;
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char answer[4];

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    printf("Ваші виконувані C-програми:\n");

    while ((entry = readdir(dir)) != NULL) {
        if (stat(entry->d_name, &st) == -1)
            continue;

        // Виконуваний файл, не директорія, не *.c, не прихований
        if (S_ISREG(st.st_mode) &&
            is_executable_by_user(&st) &&
            !ends_with(entry->d_name, ".c") &&
            entry->d_name[0] != '.') {

            printf("Файл: %s\n", entry->d_name);
            printf("Надати дозвіл на читання для інших користувачів? (y/n): ");
            fgets(answer, sizeof(answer), stdin);

            if (answer[0] == 'y' || answer[0] == 'Y') {
                mode_t new_mode = st.st_mode | S_IROTH;
                if (chmod(entry->d_name, new_mode) == 0) {
                    printf("Дозвіл надано ✅\n");
                } else {
                    perror("chmod");
                }
            }
        }
    }

    closedir(dir);
    return 0;
}
