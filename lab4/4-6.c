#include <stdio.h>
#include <stdlib.h>

int main() {
    // realloc with NULL
    void *ptr1 = realloc(NULL, 20); // аналог malloc(20)
    if (ptr1)
        printf("realloc(NULL, 20): success at %p\n", ptr1);
    else
        perror("realloc(NULL, 20) failed");

    // realloc with 0 size
    void *ptr2 = malloc(20);
    if (!ptr2) {
        perror("Initial malloc failed");
        return 1;
    }

    printf("Before realloc(ptr, 0): %p\n", ptr2);
    void *ptr3 = realloc(ptr2, 0); // аналог free(ptr2)
    printf("After realloc(ptr, 0): %p\n", ptr3);

    // NOTE: ptr3 may be NULL or some unique value; do not use it
    // No need to free it again

    // realloc(NULL, 0): result is implementation-defined
    void *ptr4 = realloc(NULL, 0);
    printf("realloc(NULL, 0): %p (may be NULL or valid ptr)\n", ptr4);
    free(ptr4); // free only if not NULL

    return 0;
}
