#if !defined(MMA_MMA_H)
#define MMA_MMA_H

#include <mpi.h>

#include <mma_export.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct mma_comm {
    int rank;
    int size;
    int my_rank0;
    int other_rank0;
    int sub_rank;
    int sub_size;
    int comm_f;
    int sub_comm_f;
    char* name;
    MPI_Comm comm;
    MPI_Comm sub_comm;
};

MMA_EXPORT int mma_initialize();
MMA_EXPORT int mma_finalize();
MMA_EXPORT int mma_print();
MMA_EXPORT int mma_print_collective(MPI_Comm comm);
MMA_EXPORT int mma_comm_register(const char* comm_name);
MMA_EXPORT int mma_comm_get(const char* comm_name, struct mma_comm** comm);
MMA_EXPORT int mma_set_name(const char* name);

#if defined(__cplusplus)
}
#endif

#endif
