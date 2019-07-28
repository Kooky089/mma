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

typedef struct _p_MMAComm* MMAComm;
struct _p_MMAComm {
    int       rank;
    int       size;
    int       myRank0;
    int       otherRank0;
    int       subRank;
    int       subSize;
    int       comm_f;
    int       subComm_f;
    char*     name;
    MPI_Comm  comm;
    MPI_Comm  subComm;
};

DLLEXPOT int MMACommGet(const char* comm_name, MMAComm* comm);
DLLEXPOT int MMACommRegister(const char* comm_name);
DLLEXPOT int MMACommPrint();
DLLEXPOT int MMACommFinalize();
DLLEXPOT int MMACommInitialize();

/* Fortran wrapper */
DLLEXPOT void mmacommget_f(char* comm_name, MMAComm* comm, int* ierror);
DLLEXPOT void mmacommregister_f(char* comm_name, int* ierror);
DLLEXPOT void mmacommprint_f(int* ierror);
DLLEXPOT void mmacommfinalize_f(int* ierror);
DLLEXPOT void mmacomminitialize_f(int* ierror);


#if defined(__cplusplus)
}
#endif


#endif