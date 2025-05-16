#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void list_dir(const char *path) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror(path);
        return;
    }

    printf("\n%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        struct stat statbuf;

        // Пропускаємо "." і ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror(fullpath);
            continue;
        }

        printf("%s\n", entry->d_name);

        // Якщо це директорія — рекурсивно обходимо її
        if (S_ISDIR(statbuf.st_mode)) {
            list_dir(fullpath);
        }
    }

    closedir(dir);
}

int main() {
    list_dir(".");
    return 0;
}
