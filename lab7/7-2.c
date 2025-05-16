#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

void print_permissions(mode_t mode) {
    char perms[11] = "----------";

    if (S_ISDIR(mode)) perms[0] = 'd';
    else if (S_ISLNK(mode)) perms[0] = 'l';
    else if (S_ISCHR(mode)) perms[0] = 'c';
    else if (S_ISBLK(mode)) perms[0] = 'b';
    else if (S_ISFIFO(mode)) perms[0] = 'p';
    else if (S_ISSOCK(mode)) perms[0] = 's';

    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';

    printf("%s", perms);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char time_str[64];

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Пропускаємо . і ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        print_permissions(file_stat.st_mode);

        printf(" %ld", (long)file_stat.st_nlink);

        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);

        printf(" %s", pw ? pw->pw_name : "unknown");
        printf(" %s", gr ? gr->gr_name : "unknown");

        printf(" %5ld", (long)file_stat.st_size);

        struct tm *tm_info = localtime(&file_stat.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", tm_info);
        printf(" %s", time_str);

        printf(" %s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
