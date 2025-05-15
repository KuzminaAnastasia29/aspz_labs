#include <stdlib.h>
#include <stdio.h>

int main() {
    void *ptr = malloc(10);
    if (!ptr) {
        perror("malloc failed");
        return 1;
    }

    for (int i = 0; i < 3; ++i) {
        printf("Using memory at iteration %d\n", i+1);
        ((char*)ptr)[0] = 'A'; // використання пам'яті
    }

    free(ptr); // звільнення після завершення використання
    return 0;
}
