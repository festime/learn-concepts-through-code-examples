#include "buffer.h"

int buffer[BUFFER_SIZE];
int fill = 0;
int use = 0;

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % BUFFER_SIZE;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % BUFFER_SIZE;
    return tmp;
}

void print_buffer(void) {
    int i;
    for (i = 0; i < BUFFER_SIZE; ++i) {
      printf("%d, ", buffer[i]);
    }
    printf("\n");
}
