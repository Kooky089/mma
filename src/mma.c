#include <mma/mma.h>
#include <mma_config.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_list.h"

#if defined(HAVE_WINDOWS_H)
#include <windows.h>
#endif

static int world_rank = MPI_PROC_NULL;
static int world_size = 0;
static struct string_list* comm_name_list = NULL;
static struct mma_comm** comm_array = NULL;
static int comm_array_size = 0;
static int initialized = 0;
static char* _name = NULL;

static int mma_get_id_(int* id);
static int mma_get_name_(const char** name);

static int mma_reset_() {
    world_rank = MPI_PROC_NULL;
    world_size = 0;
    comm_name_list = NULL;
    comm_array = NULL;
    comm_array_size = 0;
    initialized = 0;
    return 0;
}

int mma_set_name(const char* name) {
    assert(name);
    free(_name);
    _name = NULL;
    return string_trim(name, &_name);
}

int mma_comm_get(const char* comm_name, struct mma_comm** comm) {
    if (comm_array_size) {
        /* check if mma_initialize() has been called */
        char* comm_name_ = NULL;
        string_trim(comm_name, &comm_name_);
        int index = string_list_index_of(comm_name_list, comm_name_);
        free(comm_name_);
        if (index == -1) {
            /* communicator name does not exist */
            *comm = NULL;
            return 1;
        }
        *comm = comm_array[index];
        return 0;
    }
    *comm = NULL;
    return 2;
}

int mma_comm_register(const char* comm_name) {
    if (comm_array_size) {
        /* too late, MMAComms already initialized */
        return 1;
    }
    if (!comm_name_list) {
        string_list_create(&comm_name_list);
    }
    char* comm_name_ = NULL;
    string_trim(comm_name, &comm_name_);
    int result = string_list_add(comm_name_list, comm_name_);
    free(comm_name_);
    return result;
}

int mma_print() {
    int i;
    if (comm_array_size) {
        for (i = 0; i < comm_array_size; ++i) {
            printf("rank: %d, size: %d, myRank0: %d, otherRank0: %d, subRank: "
                   "%d, subSize: %d, name: %s\n",
                   comm_array[i]->rank, comm_array[i]->size, comm_array[i]->my_rank0,
                   comm_array[i]->other_rank0, comm_array[i]->sub_rank, comm_array[i]->sub_size,
                   comm_array[i]->name);
        }
    } else {
        if (comm_name_list) {
            string_list_print(comm_name_list);
        } else {
            printf("MMA: nothing to print\n");
        }
    }
    return 0;
}

int mma_print_collective(MPI_Comm comm) {
    int current_rank;
    int rank;
    int size;
    int ierror = 0;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    for (current_rank = 0; current_rank < size; current_rank++) {
        if (current_rank == rank) {
            ierror = mma_print();
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    return ierror;
}

int mma_finalize() {
    int i;
    if (initialized == 1) {
        MPI_Finalize();
    }
    if (comm_name_list) {
        string_list_destroy(&comm_name_list);
    }
    if (comm_array_size) {
        for (i = 0; i < comm_array_size; ++i) {
            free(comm_array[i]->name);
            free(comm_array[i]);
        }
        free(comm_array);
    }
    free(_name);
    mma_reset_();
    return 0;
}

static int mma_get_id_(int* exe_id) {
    struct string_list* exe_name_list;
    MPI_Status status;
    int i;
    int msg_size;
    int list_size;
    char* string;
    const char* exe_name;

    *exe_id = -1;

    /* Get exe ID */
    mma_get_name_(&exe_name);

    if (!world_rank) {
        string_list_create(&exe_name_list);
        string_list_add(exe_name_list, exe_name);
        /* Receive exe name of each process (except of itself) */
        for (i = 1; i < world_size; ++i) {
            MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &msg_size);
            string = calloc(msg_size, sizeof(char));
            if (string == NULL) {
                return 1;
            }
            MPI_Recv(string, msg_size, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            string_list_add(exe_name_list, string);
            free(string);
        }

        /* count unique exe names and tell other processes*/
        list_size = string_list_size(exe_name_list);
        MPI_Bcast(&list_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

        /* distribute all unique exe names in sequence that others know their
         * ID*/
        for (i = 0; i < list_size; i++) {
            string = string_list_get(exe_name_list, i);
            msg_size = (int)strlen(string) + 1;
            MPI_Bcast(&msg_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
            MPI_Bcast(string, msg_size, MPI_CHAR, 0, MPI_COMM_WORLD);

            if (!strcmp(string, exe_name)) {
                *exe_id = i;
            }
        }
        string_list_destroy(&exe_name_list);

    } else {
        /* send exe name to rank0 */
        MPI_Send(exe_name, (int)strlen(exe_name) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        /* receive number of unique exe names */
        MPI_Bcast(&list_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
        /* receive all exe names and determine own exe ID */
        for (i = 0; i < list_size; ++i) {
            MPI_Bcast(&msg_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
            string = malloc(sizeof(char) * msg_size);
            if (!string) {
                return 1;
            }
            MPI_Bcast(string, msg_size, MPI_CHAR, 0, MPI_COMM_WORLD);
            if (!strcmp(string, exe_name)) {
                *exe_id = i;
            }
            free(string);
        }
    }
    assert(*exe_id != -1);
    return 0;
}

static int mma_get_name_(const char** name) {
    int ierror = 0;
#if defined(HAVE_WINDOWS_H)
    char __progname[MAX_PATH];
    GetModuleFileName(NULL, __progname, MAX_PATH);
#else
    extern const char* __progname;
#endif
    if (!_name) {
        ierror = mma_set_name(__progname);
    }
    *name = _name;
    return ierror;
}

int mma_initialize() {
    struct string_list* global_comm_name_list = NULL;
    int comm_size = 0;
    int global_comm_size;
    MPI_Status status;
    MPI_Comm tmp_comm;
    int msg_size;
    char* string;
    int i;
    int j;
    int local_components;
    int global_components;
    int min_rank;
    int exe_id;
    int already_initialized_mpi;

    MPI_Initialized(&already_initialized_mpi);

    if (initialized) {
        return !already_initialized_mpi;
    }

    if (!already_initialized_mpi) {
        MPI_Init(NULL, NULL);
        initialized = 1;
    } else {
        initialized = 2;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    mma_comm_register("world");

    mma_get_id_(&exe_id);

    /* number of local registered comms */
    comm_size = string_list_size(comm_name_list);
    comm_array_size = comm_size;
    comm_array = calloc(comm_array_size, sizeof(struct mma_comm*));

    if (comm_array == NULL) {
        return 2;
    }
    for (i = 0; i < comm_array_size; ++i) {
        comm_array[i] = calloc(1, sizeof(**comm_array));
        if (comm_array[i] == NULL) {
            return 3;
        }
    }
    /* Get total number of communicator names (will have many duplicates)*/
    MPI_Reduce(&comm_size, &global_comm_size, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!world_rank) {
        /*
        Rank 0 collects a list of all communicator names
        */

        string_list_create(&global_comm_name_list);
        string_list_add_all(global_comm_name_list, comm_name_list);

        for (i = 0; i < global_comm_size - comm_size; ++i) {
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &msg_size);
            string = malloc(sizeof(char) * msg_size);
            if (!string) {
                return 4;
            }
            MPI_Recv(string, msg_size, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            string_list_add(global_comm_name_list, string);
            free(string);
        }
        /* number of global comms */
        global_comm_size = string_list_size(global_comm_name_list);

    } else {
        /* Send communicator names to root node */
        for (i = 0; i < comm_size; ++i) {
            string = string_list_get(comm_name_list, i);
            MPI_Send(string, (int)strlen(string) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    /*
    ! Root sends total number communicator names to all processes
    ! => equals number of MPI_COMM_SPLITs on group_list(0)->comm
    */
    MPI_Bcast(&global_comm_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

    for (i = 0; i < global_comm_size; ++i) {
        if (!world_rank) {
            string = string_list_get(global_comm_name_list, i);
            msg_size = (int)strlen(string) + 1;
            MPI_Bcast(&msg_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
        } else {
            MPI_Bcast(&msg_size, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
            string = malloc(sizeof(char) * msg_size);
            if (!string) {
                return 5;
            }
        }
        MPI_Bcast(string, msg_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        /* check if process wants to join comm */
        j = string_list_index_of(comm_name_list, string);

        if (j >= 0) {
            /* Local process joins the current communicator */
            MPI_Comm_split(MPI_COMM_WORLD, 0, exe_id * world_size + world_rank,
                           &comm_array[j]->comm);
            MPI_Comm_rank(comm_array[j]->comm, &comm_array[j]->rank);
            MPI_Comm_size(comm_array[j]->comm, &comm_array[j]->size);

            /* Split current group communicator into local and remote
             * communicator be exe_id */
            MPI_Comm_split(comm_array[j]->comm, exe_id, comm_array[j]->rank,
                           &comm_array[j]->sub_comm);
            MPI_Comm_rank(comm_array[j]->sub_comm, &comm_array[j]->sub_rank);
            MPI_Comm_size(comm_array[j]->sub_comm, &comm_array[j]->sub_size);

            MPI_Comm_set_name(comm_array[j]->comm, string);
            MPI_Comm_set_name(comm_array[j]->sub_comm, string);

            comm_array[j]->name = malloc(sizeof(char) * (strlen(string) + 1));
            memcpy(comm_array[j]->name, string, sizeof(char) * (strlen(string) + 1));

            /* Count the number of different work_ids in the current group_comm
             */
            if (!comm_array[j]->sub_rank) {
                local_components = 1;
            } else {
                local_components = 0;
            }
            MPI_Allreduce(&local_components, &global_components, 1, MPI_INTEGER, MPI_SUM,
                          comm_array[j]->comm);

            /* If current communicator only consists of local ranks, delete
             * group communicator. => User may still use self_comm */
            if (global_components == 1) {
                comm_array[j]->my_rank0 = 0;
                comm_array[j]->other_rank0 = MPI_PROC_NULL;

                /* group_comm consists of two workgroups */
            } else if (global_components == 2) {
                MPI_Allreduce(&exe_id, &min_rank, 1, MPI_INTEGER, MPI_MIN, comm_array[j]->comm);
                if (exe_id == min_rank) {
                    comm_array[j]->my_rank0 = 0;
                    comm_array[j]->other_rank0 = comm_array[j]->sub_size;
                } else {
                    comm_array[j]->my_rank0 = comm_array[j]->size - comm_array[j]->sub_size;
                    comm_array[j]->other_rank0 = 0;
                }

            } else {
                /*!TODO?: More than two exeIds/workgroups per group communicator.
                 * => otherRank0 wont be available*/
                comm_array[j]->my_rank0 = comm_array[j]->rank - comm_array[j]->sub_rank;
                comm_array[j]->other_rank0 = MPI_PROC_NULL;
            }

            comm_array[j]->comm_f = MPI_Comm_c2f(comm_array[j]->comm);
            comm_array[j]->sub_comm_f = MPI_Comm_c2f(comm_array[j]->sub_comm);

        } else {
            MPI_Comm_split(MPI_COMM_WORLD, 1, world_rank, &tmp_comm);
            MPI_Comm_free(&tmp_comm);
        }
        if (world_rank) {
            free(string);
        }
    }
    if (!world_rank) {
        string_list_destroy(&global_comm_name_list);
    }
    return 0;
}
