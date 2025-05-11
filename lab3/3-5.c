#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096  // 4 КБ

void handle_file_size_limit(int sig) {
    printf("Exceeded file size limit. Aborting copy.\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    const char *source = argv[1];
    const char *target = argv[2];

    // Встановлення обробника сигналу перевищення розміру файлу
    signal(SIGXFSZ, handle_file_size_limit);

    // Відкриття вхідного файлу для читання
    FILE *in = fopen(source, "rb");
    if (!in) {
        fprintf(stderr, "Cannot open file %s for reading: %s\n", source, strerror(errno));
        return 1;
    }

    // Відкриття вихідного файлу для запису
    FILE *out = fopen(target, "wb");
    if (!out) {
        fprintf(stderr, "Cannot open file %s for writing: %s\n", target, strerror(errno));
        fclose(in);
        return 1;
    }

    // Копіювання вмісту
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
        if (fwrite(buffer, 1, bytesRead, out) != bytesRead) {
            fprintf(stderr, "Write error or file size limit exceeded\n");
            fclose(in);
            fclose(out);
            return 1;
        }
    }

    fclose(in);
    fclose(out);

    printf("File copied successfully.\n");
    return 0;
}
