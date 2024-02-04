#include <stdio.h>

int main() {
    FILE *f = fopen("transmission.bin", "rb");

    if (!f) {
        perror("Could not open file");
        return 1;
    }

    char c;
    while (!feof(f)) {
        c = fgetc(f);
        printf("%02X ", c);
    }

    printf("\n");

    return 0;
}