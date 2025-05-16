#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "test.bin";
    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};

    // Створимо файл та запишемо туди 10 байтів
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open for write");
        return 1;
    }
    write(fd, data, sizeof(data));
    close(fd);

    // Тепер відкриємо файл для читання
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open for read");
        return 1;
    }

    unsigned char buffer[4];

    // Перемістимося на третій байт
    lseek(fd, 3, SEEK_SET);
    read(fd, buffer, 4);

    // Виведемо прочитані байти
    printf("Read bytes: ");
    for (int i = 0; i < 4; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}
