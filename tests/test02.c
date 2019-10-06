#include "mma/mma.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// alway use asserts for this test
#if defined(NDEBUG)
#undef NDEBUG
#endif
#include <assert.h>

// Following call must not fail to pass test02:
// mpiexec -n 3 ./test02 : -n 2 ./test02a : -n 2 ./test02b
//
// Corresponding call with test01
// mpiexec -n 1 ./test01 a b    : -n 2 ./test01 a c  : -n 2 ./test01a b d : -n 2 ./test01b a d
//

int main(int argc, char* argv[]) {

    int world_rank;
    int world_size;

    int error;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        error = mma_comm_register("a");
        assert(error == 0);
        error = mma_comm_register("b");
        assert(error == 0);
        error = mma_comm_register("a");
        assert(error != 0);
    }
    if (world_rank == 1 || world_rank == 2) {
        error = mma_comm_register("a");
        assert(error == 0);
        error = mma_comm_register("c");
        assert(error == 0);
    }
    if (world_rank == 3 || world_rank == 4) {
        error = mma_comm_register("b");
        assert(error == 0);
        error = mma_comm_register("d");
        assert(error == 0);
    }
    if (world_rank == 5 || world_rank == 6) {
        error = mma_comm_register("a");
        assert(error == 0);
        error = mma_comm_register("d");
        assert(error == 0);
    }

    struct mma_comm* test_error_comm;
    // ask for comm before initialized
    error = mma_comm_get("a", &test_error_comm);
    assert(error != 0);
    // ask for comm that doesnt exist
    error = mma_comm_get("ab", &test_error_comm);
    assert(error != 0);

    error = mma_initialize();
    assert(error == 0);
    struct mma_comm* world;
    error = mma_comm_get("world", &world);
    assert(error == 0);

    assert(world->rank == world_rank);
    assert(world->size == world_size);
    assert(world->other_rank0 == MPI_PROC_NULL);

    if (world_rank == 0) {
        struct mma_comm* a;
        error = mma_comm_get("a", &a);
        assert(error == 0);
        assert(a->rank == 0);
        assert(a->size == 5);
        assert(a->sub_rank == 0);
        assert(a->sub_size == 3);
        assert(a->my_rank0 == 0);
        assert(a->other_rank0 == 3);
        assert(!strcmp(a->name, "a"));
        struct mma_comm* b;
        error = mma_comm_get("b", &b);
        assert(error == 0);
        assert(b->rank == 0);
        assert(b->size == 3);
        assert(b->sub_rank == 0);
        assert(b->sub_size == 1);
        assert(b->my_rank0 == 0);
        assert(b->other_rank0 == 1);
        assert(!strcmp(b->name, "b"));
    }
    if (world_rank == 1) {
        struct mma_comm* a;
        error = mma_comm_get("a", &a);
        assert(error == 0);
        assert(a->rank == 1);
        assert(a->size == 5);
        assert(a->sub_rank == 1);
        assert(a->sub_size == 3);
        assert(a->my_rank0 == 0);
        assert(a->other_rank0 == 3);
        assert(!strcmp(a->name, "a"));
        struct mma_comm* c;
        error = mma_comm_get("c", &c);
        assert(error == 0);
        assert(c->rank == 0);
        assert(c->size == 2);
        assert(c->sub_rank == 0);
        assert(c->sub_size == 2);
        assert(c->my_rank0 == 0);
        assert(c->other_rank0 == MPI_PROC_NULL);
        assert(!strcmp(c->name, "c"));
    }
    if (world_rank == 2) {
        struct mma_comm* a;
        error = mma_comm_get("a", &a);
        assert(error == 0);
        assert(a->rank == 2);
        assert(a->size == 5);
        assert(a->sub_rank == 2);
        assert(a->sub_size == 3);
        assert(a->my_rank0 == 0);
        assert(a->other_rank0 == 3);
        assert(!strcmp(a->name, "a"));
        struct mma_comm* c;
        error = mma_comm_get("c", &c);
        assert(error == 0);
        assert(c->rank == 1);
        assert(c->size == 2);
        assert(c->sub_rank == 1);
        assert(c->sub_size == 2);
        assert(c->my_rank0 == 0);
        assert(c->other_rank0 == MPI_PROC_NULL);
        assert(!strcmp(c->name, "c"));
    }
    if (world_rank == 3) {
        struct mma_comm* b;
        error = mma_comm_get("b", &b);
        assert(error == 0);
        assert(b->rank == 1);
        assert(b->size == 3);
        assert(b->sub_rank == 0);
        assert(b->sub_size == 2);
        assert(b->my_rank0 == 1);
        assert(b->other_rank0 == 0);
        assert(!strcmp(b->name, "b"));
        struct mma_comm* d;
        error = mma_comm_get("d", &d);
        assert(error == 0);
        assert(d->rank == 0);
        assert(d->size == 4);
        assert(d->sub_rank == 0);
        assert(d->sub_size == 2);
        assert(d->my_rank0 == 0);
        assert(d->other_rank0 == 2);
        assert(!strcmp(d->name, "d"));
    }
    if (world_rank == 4) {
        struct mma_comm* b;
        error = mma_comm_get("b", &b);
        assert(error == 0);
        assert(b->rank == 2);
        assert(b->size == 3);
        assert(b->sub_rank == 1);
        assert(b->sub_size == 2);
        assert(b->my_rank0 == 1);
        assert(b->other_rank0 == 0);
        assert(!strcmp(b->name, "b"));
        struct mma_comm* d;
        error = mma_comm_get("d", &d);
        assert(error == 0);
        assert(d->rank == 1);
        assert(d->size == 4);
        assert(d->sub_rank == 1);
        assert(d->sub_size == 2);
        assert(d->my_rank0 == 0);
        assert(d->other_rank0 == 2);
        assert(!strcmp(d->name, "d"));
    }
    if (world_rank == 5) {
        struct mma_comm* a;
        error = mma_comm_get("a", &a);
        assert(error == 0);
        assert(a->rank == 3);
        assert(a->size == 5);
        assert(a->sub_rank == 0);
        assert(a->sub_size == 2);
        assert(a->my_rank0 == 3);
        assert(a->other_rank0 == 0);
        assert(!strcmp(a->name, "a"));
        struct mma_comm* d;
        error = mma_comm_get("d", &d);
        assert(error == 0);
        assert(d->rank == 2);
        assert(d->size == 4);
        assert(d->sub_rank == 0);
        assert(d->sub_size == 2);
        assert(d->my_rank0 == 2);
        assert(d->other_rank0 == 0);
        assert(!strcmp(d->name, "d"));
    }
    if (world_rank == 6) {
        struct mma_comm* a;
        error = mma_comm_get("a", &a);
        assert(error == 0);
        assert(a->rank == 4);
        assert(a->size == 5);
        assert(a->sub_rank == 1);
        assert(a->sub_size == 2);
        assert(a->my_rank0 == 3);
        assert(a->other_rank0 == 0);
        assert(!strcmp(a->name, "a"));
        struct mma_comm* d;
        error = mma_comm_get("d", &d);
        assert(error == 0);
        assert(d->rank == 3);
        assert(d->size == 4);
        assert(d->sub_rank == 1);
        assert(d->sub_size == 2);
        assert(d->my_rank0 == 2);
        assert(d->other_rank0 == 0);
        assert(!strcmp(d->name, "d"));
    }
    error = mma_print_collective(MPI_COMM_WORLD);
    assert(error == 0);
    error = mma_finalize();
    assert(error == 0);
    return 0;
}
