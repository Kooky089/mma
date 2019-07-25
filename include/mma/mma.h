#if !defined(__MMA_H)
#define __MMA_H

#include<mpi.h>

#if defined(_WIN32)
#define DllExport   __declspec( dllexport )
#else
#define DllExport
#endif


#ifdef __cplusplus
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
    char     *name;
    MPI_Comm  comm;
    MPI_Comm  subComm;
};

DllExport int MMACommGet(char const *, MMAComm *);
DllExport int MMACommRegister(char const *);
DllExport int MMACommPrint();
DllExport int MMACommFinalize();
DllExport int MMACommInitialize();

/* Fortran wrapper */
DllExport void mmacommget_f(char *string, MMAComm *comm, int *ierror);
DllExport void mmacommregister_f(char *string, int *ierror);
DllExport void mmacommprint_f(int *ierror);
DllExport void mmacommfinalize_f(int *ierror);
DllExport void mmacomminitialize_f(int *ierror);


#ifdef __cplusplus
}
#endif


#endif