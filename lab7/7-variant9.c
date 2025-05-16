#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int check_port(const char *ip, int port) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "nc -z -w1 %s %d 2>&1", ip, port);
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("popen");
        return 0;
    }

    char output[256];
    int open = 0;
    while (fgets(output, sizeof(output), fp) != NULL) {
        if (strstr(output, "succeeded") != NULL || strstr(output, "open") != NULL) {
            open = 1;
            break;
        }
    }
    pclose(fp);
    return open;
}

int main() {
    const char *ip = "192.168.1.1";
    int port = 80;

    if (check_port(ip, port)) {
        printf("Порт %d на %s відкритий\n", port, ip);
    } else {
        printf("Порт %d на %s закритий\n", port, ip);
    }
    return 0;
}
