#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_DIRS 1024

int is_directory(const char *name) {
    struct stat statbuf;
    if (stat(name, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    char *directories[MAX_DIRS];
    int count = 0;

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Пропускаємо . і ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (is_directory(entry->d_name)) {
            directories[count] = strdup(entry->d_name);  // копіюємо ім’я
            count++;
        }
    }

    closedir(dir);

    qsort(directories, count, sizeof(char *), compare);

    for (int i = 0; i < count; i++) {
        printf("%s\n", directories[i]);
        free(directories[i]);  // звільнення пам’яті
    }

    return 0;
}
