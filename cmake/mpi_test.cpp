#include <mpi.h>
#include <iostream>

int main() {
    MPI_Init(nullptr, nullptr);
    MPI_Finalize();
    return 0;
}