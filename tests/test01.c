#include "mma/mma.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        mma_comm_register(argv[i]);
    }
    mma_initialize();
    mma_print();
    mma_finalize();
    return 0;
}
