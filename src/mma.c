#include "mma/mma.h"
#include "StringSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
  #include <Windows.h>
#else
#endif

static int          worldRank = MPI_PROC_NULL, worldSize = 0;
static StringSet    *commNameList = NULL;
static MMAComm      *commArray = NULL;
static int          commArraySize = 0;
static int          initialized = 0;

static int MMACommGetExeId_private(int *exeId);
static int MMACommGetExeName_private(char **name);

static int MMACommReset_private() {
  worldRank = MPI_PROC_NULL;
  worldSize = 0;
  commNameList = NULL;
  commArray = NULL;
  commArraySize = 0;
  initialized = 0;
  return 0;
}

int MMACommGet(char const *commName, MMAComm *comm) {
  if (commArraySize) {
    // check if MMACommInitialize() has been called
    int index = StringSetIndexOf(commNameList,commName);
    if (index == -1) {
      // communicator name does not exist
      *comm = NULL;
      return 1;
    }
    *comm = commArray[index];
    return 0;
  }
  *comm = NULL;
  return 2;
}

int MMACommRegister(char const *commName) {
  if (commArraySize) {
    // too late, MMAComms already initialized
    return 1;
  }
  if (!commNameList) {
    StringSetCreate(&commNameList);
  }
  return StringSetAdd(commNameList,commName);
}

int MMACommPrint() {
  int i;
  if (commArraySize) {
    for (i=0; i<commArraySize; i++) {
      printf("rank: %d, size: %d, myRank0: %d, otherRank0: %d, subRank: %d, subSize: %d, name: %s\n",
        commArray[i]->rank,
        commArray[i]->size,
        commArray[i]->myRank0,
        commArray[i]->otherRank0,
        commArray[i]->subRank,
        commArray[i]->subSize,
        commArray[i]->name);
    }
  } else {
    if (commNameList) {
      StringSetPrint(commNameList);
    } else {
      printf("MMA: nothing to print\n");
    }
  }
  return 0;
}



int MMACommFinalize() {
  int i;
  if (initialized == 1) {
    MPI_Finalize();
  }
  if (commNameList) {
    StringSetDestroy(&commNameList);
  }
  if (commArraySize) {
    for (i=0; i<commArraySize; i++) {
      free(commArray[i]->name);
      free(commArray[i]);
    }
    free(commArray);
  }
  MMACommReset_private();
  return 0;
}

static int MMACommGetExeId_private(int *exeId) {
  StringSet *exeNameList;
  MPI_Status status;
  int i;
  int msgSize;
  int listSize;
  char *string;
  char *exeName;
  
  /* Get exe ID */
  MMACommGetExeName_private(&exeName);
  
  if (!worldRank) {
    StringSetCreate(&exeNameList);
    StringSetAdd(exeNameList,exeName);
    /* Receive exe name of each process (except of itself) */
    for (i=1; i<worldSize; i++) {
      MPI_Probe(i,0,MPI_COMM_WORLD,&status);
      MPI_Get_count(&status,MPI_CHAR,&msgSize);
      string = calloc(msgSize,sizeof(char));
      if (string == NULL) {
        return 1;
      }
      MPI_Recv(string,msgSize,MPI_CHAR,status.MPI_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      StringSetAdd(exeNameList,string);
      free(string);
    }
    
    /* count unique exe names and tell other processes*/
    listSize = StringSetSize(exeNameList);
    MPI_Bcast(&listSize, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
    
    /* distribute all unique exe names in sequence that others know their ID*/
    for (i=0; i<listSize; i++) {
      string = StringSetGet(exeNameList,i);
      msgSize = (int)strlen(string)+1;
      MPI_Bcast(&msgSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
      MPI_Bcast(string,msgSize,MPI_CHAR,0,MPI_COMM_WORLD);
      
      if (!strcmp(string,exeName)) {
        *exeId = i;
      }
    }
    StringSetDestroy(&exeNameList);
    
  } else {
    /* send exe name to rank0 */
    MPI_Send(exeName,(int)strlen(exeName)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
    /* receive number of unique exe names */
    MPI_Bcast(&listSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    /* receive all exe names and determine own exe ID */
    for (i=0; i<listSize; i++) {
      MPI_Bcast(&msgSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
      string = malloc(sizeof(char)*msgSize);
      if (string == NULL) {
        return 1;
      }
      MPI_Bcast(string,msgSize,MPI_CHAR,0,MPI_COMM_WORLD);
      if (!strcmp(string, exeName)) {
        *exeId = i;
      }
      free(string);
    }
  }
  free(exeName);
  return 0;
}

static int MMACommGetExeName_private(char **name) {
#if defined(_WIN32)
  char __progname[MAX_PATH]; 
  GetModuleFileName(NULL, __progname, MAX_PATH);
#else
  extern const char *__progname;
#endif
  *name = malloc(sizeof(char)*strlen(__progname)+1);
  if (name == NULL) {
	  return 1;
  }
  memcpy(*name, __progname,sizeof(char)*strlen(__progname)+1);
  return 0;
}

int MMACommInitialize() {
  StringSet   *globalCommNameList;
  int         commSize = 0;
  int         globalCommSize;
  MPI_Status  status;
  MPI_Comm    tmpComm;
  int         msgSize;
  char        *string;
  int         i, j;
  int         localComponents;
  int         globalComponents;
  int         minRank;
  int         exeId;
  int         alreadyInitializedMPI;
  
  MPI_Initialized(&alreadyInitializedMPI);
  
  if (initialized) {
    return !alreadyInitializedMPI;
  }
  
  if (!alreadyInitializedMPI) {
    MPI_Init(NULL, NULL);
    initialized = 1;
  } else {
    initialized = 2;
  }
  
  MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
  MPI_Comm_size(MPI_COMM_WORLD,&worldSize);
 
  MMACommRegister("world");
  
  MMACommGetExeId_private(&exeId);
   
  /* number of local registered comms */
  commSize = StringSetSize(commNameList);
  commArraySize = commSize;
  commArray = calloc(commArraySize,sizeof(MMAComm));
  if (commArray == NULL) {
	  return 2;
  }
  for (i=0; i<commArraySize; i++) {
    commArray[i] = calloc(1,sizeof(**commArray));
    if (commArray[i] == NULL) {
      return 3;
    }
  }
  
  /* Get total number of communicator names (will have many duplicates)*/
  MPI_Reduce(&commSize,&globalCommSize,1,MPI_INTEGER, MPI_SUM,0,MPI_COMM_WORLD);

  if (!worldRank) {
    /*
    Rank 0 collects a list of all communicator names
    */
    
    StringSetCreate(&globalCommNameList);
    StringSetAddAll(globalCommNameList, commNameList);
    
    for (i=0; i<globalCommSize-commSize; i++) {
      MPI_Probe(MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
      MPI_Get_count(&status,MPI_CHAR,&msgSize);
      string = malloc(sizeof(char)*msgSize);
      if (string == NULL) {
        return 4;
      }
      MPI_Recv(string,msgSize,MPI_CHAR,status.MPI_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      StringSetAdd(globalCommNameList,string);
      free(string);
    }
    /* number of global comms */
    globalCommSize = StringSetSize(globalCommNameList);
    
  } else {
    /* Send communicator names to root node */
    for (i=0; i<commSize; i++) {
      string = StringSetGet(commNameList,i);
      MPI_Send(string,(int)strlen(string)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
  }
  
  /*
  ! Root sends total number communicator names to all processes
  ! => equals number of MPI_COMM_SPLITs on group_list(0)->comm
  */
  MPI_Bcast(&globalCommSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
  
  for (i=0; i<globalCommSize; i++) {
    
    if (!worldRank) {
      string = StringSetGet(globalCommNameList,i);
      msgSize = (int)strlen(string)+1;
    }
    MPI_Bcast(&msgSize,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    if (worldRank) {
      string = malloc(sizeof(char)*msgSize);
      if (string == NULL) {
        return 5;
      }
    }
    MPI_Bcast(string,msgSize,MPI_CHAR,0,MPI_COMM_WORLD);
    
    /* check if process wants to join comm */
    j = StringSetIndexOf(commNameList,string);
      
    if (j >= 0) {
      /* Local process joins the current communicator */
      MPI_Comm_split(MPI_COMM_WORLD,0,exeId*worldSize+worldRank,&commArray[j]->comm);
      MPI_Comm_rank(commArray[j]->comm,&commArray[j]->rank);
      MPI_Comm_size(commArray[j]->comm,&commArray[j]->size);
                  
      /* Split current group communicator into local and remote communicator be exeId */
      MPI_Comm_split(commArray[j]->comm,exeId,commArray[j]->rank,&commArray[j]->subComm);
      MPI_Comm_rank(commArray[j]->subComm,&commArray[j]->subRank);
      MPI_Comm_size(commArray[j]->subComm,&commArray[j]->subSize);
      
      MPI_Comm_set_name(commArray[j]->comm,string);
      MPI_Comm_set_name(commArray[j]->subComm,string);
      
      commArray[j]->name = malloc(sizeof(char)*(strlen(string)+1));
      memcpy(commArray[j]->name,string,sizeof(char)*(strlen(string)+1));
      
      /* Count the number of different work_ids in the current group_comm */
      if (!commArray[j]->subRank) {
         localComponents = 1;
      } else {
         localComponents = 0;
      }
      MPI_Allreduce(&localComponents,&globalComponents,1,MPI_INTEGER,MPI_SUM,commArray[j]->comm);
      /* If current communicator only consists of local ranks, delete group communicator. => User may still use self_comm */
      if (globalComponents == 1) {
         commArray[j]->myRank0 = 0;
         commArray[j]->otherRank0 = MPI_PROC_NULL;
         
      /* group_comm consists of two workgroups */
      } else if (globalComponents == 2) {
         MPI_Allreduce(&exeId,&minRank,1,MPI_INTEGER,MPI_MIN,commArray[j]->comm);
         if (exeId == minRank) {
            commArray[j]->myRank0 = 0;
            commArray[j]->otherRank0 = commArray[j]->subSize;
         } else { 
            commArray[j]->myRank0 = commArray[j]->size - commArray[j]->subSize;
            commArray[j]->otherRank0 = 0;
         }
         
      } else { /*!TODO?: More than two exeIds/workgroups per group communicator. => otherRank0 wont be available*/
         commArray[j]->myRank0 = commArray[j]->rank - commArray[j]->subRank;
         commArray[j]->otherRank0 = MPI_PROC_NULL;
         
      }
      
      commArray[j]->comm_f = MPI_Comm_c2f(commArray[j]->comm);
      commArray[j]->subComm_f = MPI_Comm_c2f(commArray[j]->subComm);
      
    } else {
      MPI_Comm_split(MPI_COMM_WORLD,1,worldRank,&tmpComm);
      MPI_Comm_free(&tmpComm);
    }
    if (worldRank) {
      free(string);
    }
  } 
  if (!worldRank) {
    StringSetDestroy(&globalCommNameList);
  } 
  return 0;
}



/* Fortran wrapper */
void mmacommget_f(char *string, MMAComm *comm, int *ierror) {
	*ierror = MMACommGet(string, comm);
}
void mmacommregister_f(char *string, int *ierror) {
	*ierror = MMACommRegister(string);
}
void mmacommprint_f(int *ierror) {
	*ierror = MMACommPrint();
}
void mmacommfinalize_f(int *ierror) {
	*ierror = MMACommFinalize();
}
void mmacomminitialize_f(int *ierror) {
	*ierror = MMACommInitialize();
}
