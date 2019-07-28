#include "mma/mma.fmod"

program main
   use mma
   use mpi
   implicit none
   integer :: ierr
   type(MMAComm), pointer :: comm
   integer :: index
   integer :: myrank
   integer :: ierror
   call mma_comm_register("test", ierror)
   call mma_print(ierror)
   call mma_initialize(ierror)
   call mm_comm_get("test", comm, ierror)
   call MPI_COMM_RANK(comm%comm, myrank, ierror)
   write(*,*) comm%rank, myrank
   call mma_print(ierror)
      call mma_comm_register("test", ierror)
      write(*,*) ierror
   call mma_finalize(ierror)
end program

