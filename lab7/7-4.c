#include <stdio.h>
#include <stdlib.h>

#define PAGE_LINES 20

void wait_for_keypress() {
    printf("--More-- (натисніть Enter для продовження)");
    getchar(); // Чекаємо на натискання клавіші
}

void display_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return;
    }

    char line[1024];
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        fputs(line, stdout);
        line_count++;

        if (line_count >= PAGE_LINES) {
            wait_for_keypress();
            line_count = 0;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Використання: %s <файл1> [файл2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        printf("=== %s ===\n", argv[i]);
        display_file(argv[i]);
    }

    return 0;
}
