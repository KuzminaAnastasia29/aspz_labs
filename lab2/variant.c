#include <stdio.h>

int big_array[1000000];  // ~4MB у BSS

int main() {
    big_array[0] = 1;
    printf("BSS array first element: %d\n", big_array[0]);
    return 0;
}
