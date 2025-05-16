#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100000

int cmp_int(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return (x > y) - (x < y);
}

// Функція для вимірювання часу сортування
double measure_sort_time(int *array, size_t size) {
    int *copy = malloc(size * sizeof(int));
    memcpy(copy, array, size * sizeof(int));

    clock_t start = clock();
    qsort(copy, size, sizeof(int), cmp_int);
    clock_t end = clock();

    free(copy);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// Генератори масивів
void fill_sorted(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i)
        array[i] = i;
}

void fill_reverse(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i)
        array[i] = size - i;
}

void fill_same(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i)
        array[i] = 42;
}

void fill_random(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i)
        array[i] = rand() % 10000;
}

int main() {
    srand(time(NULL));

    int *data = malloc(MAX_SIZE * sizeof(int));
    size_t sizes[] = {1000, 5000, 10000, 50000, 100000};
    const int n_sizes = sizeof(sizes)/sizeof(sizes[0]);

    for (int i = 0; i < n_sizes; ++i) {
        size_t size = sizes[i];
        printf("Array size: %zu\n", size);

        fill_sorted(data, size);
        printf("  Sorted:     %.6f sec\n", measure_sort_time(data, size));

        fill_reverse(data, size);
        printf("  Reversed:   %.6f sec\n", measure_sort_time(data, size));

        fill_same(data, size);
        printf("  All equal:  %.6f sec\n", measure_sort_time(data, size));

        fill_random(data, size);
        printf("  Random:     %.6f sec\n\n", measure_sort_time(data, size));
    }

    free(data);
    return 0;
}
