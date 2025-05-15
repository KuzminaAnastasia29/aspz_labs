#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 64     // розмір одного блоку в байтах
#define BLOCK_COUNT 1024  // загальна кількість блоків

// Структура вузла списку вільних блоків
typedef struct FreeBlock {
    struct FreeBlock *next;
} FreeBlock;

// Статичний пул памʼяті
static uint8_t memory_pool[BLOCK_SIZE * BLOCK_COUNT];
static FreeBlock *freelist = NULL;

// Ініціалізація аллокатора
void init_allocator() {
    freelist = (FreeBlock *)memory_pool;
    FreeBlock *current = freelist;

    for (int i = 1; i < BLOCK_COUNT; i++) {
        current->next = (FreeBlock *)(memory_pool + i * BLOCK_SIZE);
        current = current->next;
    }

    current->next = NULL; // кінець списку
}

// Виділення памʼяті
void *custom_alloc() {
    if (!freelist) {
        fprintf(stderr, "custom_alloc: Немає вільної памʼяті!\n");
        return NULL;
    }

    void *block = (void *)freelist;
    freelist = freelist->next;
    return block;
}

// Звільнення памʼяті
void custom_free(void *ptr) {
    if (!ptr) return;

    FreeBlock *block = (FreeBlock *)ptr;
    block->next = freelist;
    freelist = block;
}

// Відладочна функція: підрахунок вільних блоків
void debug_allocator() {
    int count = 0;
    FreeBlock *current = freelist;
    while (current) {
        count++;
        current = current->next;
    }
    printf("Вільних блоків: %d\n", count);
}

// Тестування
int main() {
    init_allocator();
    debug_allocator(); // має бути 1024

    void *a = custom_alloc();
    void *b = custom_alloc();
    void *c = custom_alloc();

    debug_allocator(); // має бути 1021

    custom_free(b);
    custom_free(a);
    custom_free(c);

    debug_allocator(); // має бути знову 1024

    return 0;
}
