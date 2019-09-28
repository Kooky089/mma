#if !defined(_MMA_MMA_H)
#define _MMA_MMA_H

#include <mpi.h>

#if defined(_WIN32)
#define DLLEXPORT   __declspec( dllexport )
#else
#define DLLEXPORT
#endif


#if defined(__cplusplus)
extern "C" {
#endif

struct mma_comm {
    int       rank;
    int       size;
    int       my_rank0;
    int       other_rank0;
    int       sub_rank;
    int       sub_size;
    int       comm_f;
    int       sub_comm_f;
    char      *name;
    MPI_Comm  comm;
    MPI_Comm  sub_comm;
};

DLLEXPORT int mma_initialize();
DLLEXPORT int mma_finalize();
DLLEXPORT int mma_print();
DLLEXPORT int mma_comm_register(const char *comm_name);
DLLEXPORT int mma_comm_get(const char *comm_name, struct mma_comm **comm);


#if defined(__cplusplus)
}
#endif

#endif