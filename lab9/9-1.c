#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UID_THRESHOLD 500
#define MAX_LINE 1024

int is_interactive_shell(const char *shell) {
    return shell &&
           strstr(shell, "nologin") == NULL &&
           strstr(shell, "false") == NULL;
}

int main() {
    FILE *fp;
    char line[MAX_LINE];

    fp = popen("getent passwd", "r");
    if (fp == NULL) {
        perror("Не вдалося виконати getent");
        return 1;
    }

    printf("Звичайні користувачі у системі (UID >= %d, shell ≠ nologin/false):\n", UID_THRESHOLD);

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *login = strtok(line, ":");
        strtok(NULL, ":");  // password
        char *uid_str = strtok(NULL, ":");
        strtok(NULL, ":");  // gid
        strtok(NULL, ":");  // gecos
        char *home = strtok(NULL, ":");
        char *shell = strtok(NULL, ":\n");

        if (login && uid_str && shell) {
            int uid = atoi(uid_str);
            if (uid >= UID_THRESHOLD && uid != getuid() && is_interactive_shell(shell)) {
                printf("Користувач: %s, UID: %d, Дім: %s, Shell: %s\n",
                       login, uid, home ? home : "-", shell);
            }
        }
    }

    pclose(fp);
    return 0;
}
