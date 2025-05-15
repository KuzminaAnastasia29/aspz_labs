#include <stdio.h>
#include <stdlib.h>

int main() {
    int xa = 1000000;
    int xb = 5000;

    int num = xa * xb; // може призвести до переповнення

    printf("xa = %d, xb = %d\n", xa, xb);
    printf("num (int) = %d\n", num);

    void* ptr = malloc(num);

    if (ptr == NULL) {
        printf("malloc(%d) не вдалося: недостатньо пам’яті або неправильне значення\n", num);
    } else {
        printf("malloc(%d) вдалося: пам’ять виділено\n", num);
        free(ptr);
    }

    return 0;
}
