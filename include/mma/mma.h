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


/* Fortran wrapper */
DLLEXPORT void mma_initialize_fc(int *ierror);
DLLEXPORT void mma_finalize_fc(int *ierror);
DLLEXPORT void mma_print_fc(int *ierror);
DLLEXPORT void mma_comm_register_fc(const char *comm_name, int *ierror);
DLLEXPORT void mma_comm_get_fc(const char *comm_name, struct mma_comm **comm, int *ierror);




#if defined(__cplusplus)
}
#endif

#endif