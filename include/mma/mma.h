#if !defined(MMA_MMA_H_)
#define MMA_MMA_H_

#include <mpi.h>

#if defined(_WIN32)
#define DLLEXPOT   __declspec( dllexport )
#else
#define DLLEXPOT
#endif


#if defined(__cplusplus)
extern "C" {
#endif

struct mma_comm {
    int       rank;
    int       size;
    int       myRank0;
    int       otherRank0;
    int       subRank;
    int       subSize;
    int       comm_f;
    int       subComm_f;
    char      *name;
    MPI_Comm  comm;
    MPI_Comm  subComm;
};

DLLEXPOT int mma_initialize();
DLLEXPOT int mma_finalize();
DLLEXPOT int mma_print();
DLLEXPOT int mma_comm_register(const char *comm_name);
DLLEXPOT int mma_comm_get(const char *comm_name, struct mma_comm **comm);


/* Fortran wrapper */
DLLEXPOT void mma_initialize_f(int *ierror);
DLLEXPOT void mma_finalize_f(int *ierror);
DLLEXPOT void mma_print_f(int *ierror);
DLLEXPOT void mma_comm_register_f(char *comm_name, int *ierror);
DLLEXPOT void mma_comm_get_f(char *comm_name, struct mma_comm **comm, int *ierror);




#if defined(__cplusplus)
}
#endif


#endif