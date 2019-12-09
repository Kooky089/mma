#include <mpi.h>
#include <stdio.h>

int main() {
    MPI_Init(NULL, NULL);
    MPI_Finalize();
    return 0;
}