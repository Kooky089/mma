#include "mma/mma.fmod"

program main
   use mma
   use mpi
   implicit none
   type(mma_comm), pointer :: comm
   integer :: my_rank
   integer :: ierror
   call mma_comm_register("test", ierror)
   call mma_print(ierror)
   call mma_initialize(ierror)
   call mma_comm_get("test", comm, ierror)
   call MPI_COMM_RANK(comm%comm, my_rank, ierror)
   write(*,*) comm%rank, my_rank
   call mma_print(ierror)
      call mma_comm_register("test", ierror)
      write(*,*) ierror
   call mma_finalize(ierror)
end program

