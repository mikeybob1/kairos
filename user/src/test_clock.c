#include "stdio.h"
#include "unistd.h"

int main() {
    if ((fork()) == 0) { // child
        for (;;);
    } else { // parent
        wait(NULL);
    }

    return 0;
}