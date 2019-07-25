#include "mma/mma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* alway use asserts for this test */
#if defined(NDEBUG)
#undef NDEBUG
#include <assert.h>


/* Following call must not fail to pass test02:
// mpiexec -n 3 ./test02 : -n 2 ./test02a : -n 2 ./test02b
//
// Corresponding call with test01
// mpiexec -n 1 ./test01 a b    : -n 2 ./test01 a c  : -n 2 ./test01a b d : -n 2 ./test01b a d
*/

int main(int argc, char* argv[]) {

    int worldRank;
    int worldSize;

    int error;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);

    if (worldRank == 0) {
        error = MMACommRegister("a"); assert(error == 0);
        error = MMACommRegister("b"); assert(error == 0);
        error = MMACommRegister("a"); assert(error != 0);
    }
    if (worldRank == 1 || worldRank == 2) {
        error = MMACommRegister("a"); assert(error == 0);
        error = MMACommRegister("c"); assert(error == 0);
    }
    if (worldRank == 3 || worldRank == 4) {
        error = MMACommRegister("b"); assert(error == 0);
        error = MMACommRegister("d"); assert(error == 0);
    }
    if (worldRank == 5 || worldRank == 6) {
        error = MMACommRegister("a"); assert(error == 0);
        error = MMACommRegister("d"); assert(error == 0);
    }


    MMAComm testErrorComm;
    // ask for comm before initialized
    error = MMACommGet("a",&testErrorComm); assert(error != 0);
    // ask for comm that doesnt exist
    error = MMACommGet("ab",&testErrorComm); assert(error != 0);

    error = MMACommInitialize(); assert(error == 0);
    MMAComm world;
    error = MMACommGet("world", &world); assert(error == 0);

    assert(world->rank == worldRank);
    assert(world->size == worldSize);
    assert(world->otherRank0 == MPI_PROC_NULL);

    if (worldRank == 0) {
        MMAComm a;
        error = MMACommGet("a",&a); assert(error == 0);
        assert(a->rank == 0);
        assert(a->size == 5);
        assert(a->subRank == 0);
        assert(a->subSize == 3);
        assert(a->myRank0 == 0);
        assert(a->otherRank0 == 3);
        assert(!strcmp(a->name,"a"));
        MMAComm b;
        error = MMACommGet("b",&b); assert(error == 0);
        assert(b->rank == 0);
        assert(b->size == 3);
        assert(b->subRank == 0);
        assert(b->subSize == 1);
        assert(b->myRank0 == 0);
        assert(b->otherRank0 == 1);
        assert(!strcmp(b->name,"b"));
    }
    if (worldRank == 1) {
        MMAComm a;
        error = MMACommGet("a",&a); assert(error == 0);
        assert(a->rank == 1);
        assert(a->size == 5);
        assert(a->subRank == 1);
        assert(a->subSize == 3);
        assert(a->myRank0 == 0);
        assert(a->otherRank0 == 3);
        assert(!strcmp(a->name,"a"));
        MMAComm c;
        error = MMACommGet("c",&c); assert(error == 0);
        assert(c->rank == 0);
        assert(c->size == 2);
        assert(c->subRank == 0);
        assert(c->subSize == 2);
        assert(c->myRank0 == 0);
        assert(c->otherRank0 == MPI_PROC_NULL);
        assert(!strcmp(c->name,"c"));
    }
    if (worldRank == 2) {
        MMAComm a;
        error = MMACommGet("a",&a); assert(error == 0);
        assert(a->rank == 2);
        assert(a->size == 5);
        assert(a->subRank == 2);
        assert(a->subSize == 3);
        assert(a->myRank0 == 0);
        assert(a->otherRank0 == 3);
        assert(!strcmp(a->name,"a"));
        MMAComm c;
        error = MMACommGet("c",&c); assert(error == 0);
        assert(c->rank == 1);
        assert(c->size == 2);
        assert(c->subRank == 1);
        assert(c->subSize == 2);
        assert(c->myRank0 == 0);
        assert(c->otherRank0 == MPI_PROC_NULL);
        assert(!strcmp(c->name,"c"));
    }
    if (worldRank == 3) {
        MMAComm b;
        error = MMACommGet("b",&b); assert(error == 0);
        assert(b->rank == 1);
        assert(b->size == 3);
        assert(b->subRank == 0);
        assert(b->subSize == 2);
        assert(b->myRank0 == 1);
        assert(b->otherRank0 == 0);
        assert(!strcmp(b->name,"b"));
        MMAComm d;
        error = MMACommGet("d",&d); assert(error == 0);
        assert(d->rank == 0);
        assert(d->size == 4);
        assert(d->subRank == 0);
        assert(d->subSize == 2);
        assert(d->myRank0 == 0);
        assert(d->otherRank0 == 2);
        assert(!strcmp(d->name,"d"));
    }
    if (worldRank == 4) {
        MMAComm b;
        error = MMACommGet("b",&b); assert(error == 0);
        assert(b->rank == 2);
        assert(b->size == 3);
        assert(b->subRank == 1);
        assert(b->subSize == 2);
        assert(b->myRank0 == 1);
        assert(b->otherRank0 == 0);
        assert(!strcmp(b->name,"b"));
        MMAComm d;
        error = MMACommGet("d",&d); assert(error == 0);
        assert(d->rank == 1);
        assert(d->size == 4);
        assert(d->subRank == 1);
        assert(d->subSize == 2);
        assert(d->myRank0 == 0);
        assert(d->otherRank0 == 2);
        assert(!strcmp(d->name,"d"));
    }
    if (worldRank == 5) {
        MMAComm a;
        error = MMACommGet("a",&a); assert(error == 0);
        assert(a->rank == 3);
        assert(a->size == 5);
        assert(a->subRank == 0);
        assert(a->subSize == 2);
        assert(a->myRank0 == 3);
        assert(a->otherRank0 == 0);
        assert(!strcmp(a->name,"a"));
        MMAComm d;
        error = MMACommGet("d",&d); assert(error == 0);
        assert(d->rank == 2);
        assert(d->size == 4);
        assert(d->subRank == 0);
        assert(d->subSize == 2);
        assert(d->myRank0 == 2);
        assert(d->otherRank0 == 0);
        assert(!strcmp(d->name,"d"));
    }
    if (worldRank == 6) {
        MMAComm a;
        error = MMACommGet("a",&a); assert(error == 0);
        assert(a->rank == 4);
        assert(a->size == 5);
        assert(a->subRank == 1);
        assert(a->subSize == 2);
        assert(a->myRank0 == 3);
        assert(a->otherRank0 == 0);
        assert(!strcmp(a->name,"a"));
        MMAComm d;
        error = MMACommGet("d",&d); assert(error == 0);
        assert(d->rank == 3);
        assert(d->size == 4);
        assert(d->subRank == 1);
        assert(d->subSize == 2);
        assert(d->myRank0 == 2);
        assert(d->otherRank0 == 0);
        assert(!strcmp(d->name,"d"));
    }
    error = MMACommPrint(); assert(error == 0);
    error = MMACommFinalize(); assert(error == 0);
    return 0;
}
