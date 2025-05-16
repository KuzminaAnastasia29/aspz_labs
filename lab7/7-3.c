#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Використання: %s <слово> <файл>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *pattern = argv[1];
    char *filename = argv[2];
    char line[MAX_LINE];

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Не вдалося відкрити файл");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            printf("%s", line);
        }
    }

    fclose(file);
    return 0;
}
