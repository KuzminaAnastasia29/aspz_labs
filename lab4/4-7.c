#include <stdlib.h>
#include <stdio.h>

struct sbar {
    int a;
    double b;
};

int main() {
    struct sbar *ptr = calloc(1000, sizeof(struct sbar));
    if (!ptr) {
        perror("calloc failed");
        return 1;
    }

    struct sbar *newptr = reallocarray(ptr, 500, sizeof(struct sbar));
    if (!newptr) {
        perror("reallocarray failed");
        free(ptr);
        return 1;
    }

    free(newptr);
    return 0;
}
