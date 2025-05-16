#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void check_file_access(const char *path, const char *filename) {
    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);

    struct stat st;
    if (stat(fullpath, &st) != 0) {
        perror("stat");
        return;
    }

    printf("\nФайл: %s\n", fullpath);
    printf("  Власник UID: %d, GID: %d\n", st.st_uid, st.st_gid);
    printf("  Права: %o\n", st.st_mode & 0777);

    // Читання
    int fd = open(fullpath, O_RDONLY);
    if (fd != -1) {
        printf("   Можна читати\n");
        close(fd);
    } else {
        printf("   Не можна читати: %s\n", strerror(errno));
    }

    // Запис
    fd = open(fullpath, O_WRONLY);
    if (fd != -1) {
        printf("   Можна писати\n");
        close(fd);
    } else {
        printf("   Не можна писати: %s\n", strerror(errno));
    }

    // Виконання (лише якщо це файл і має x-права)
    if (access(fullpath, X_OK) == 0) {
        printf("   Можна виконувати\n");
    } else {
        printf("   Не можна виконувати: %s\n", strerror(errno));
    }
}

void scan_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Не вдалося відкрити каталог");
        return;
    }

    printf("\n Каталог: %s\n", path);
    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < 5) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            check_file_access(path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
}

int main() {
    char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Змінна HOME не визначена\n");
        return 1;
    }

    scan_directory(home);
    scan_directory("/usr/bin");
    scan_directory("/etc");

    return 0;
}
