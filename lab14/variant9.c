#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <секунди>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        fprintf(stderr, "Будь ласка, введіть додатне число секунд.\n");
        return EXIT_FAILURE;
    }

    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = 0;

    printf("Очікування %d секунд...\n", seconds);

    // Засипання без використання сигналів
    if (clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL) != 0) {
        perror("clock_nanosleep");
        return EXIT_FAILURE;
    }

    printf("Час вийшов! Минуло %d секунд.\n", seconds);
    return EXIT_SUCCESS;
}
