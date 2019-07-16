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
   call mmacommregister("test", ierror)
   call mmacommprint(ierror)
   call mmacomminitialize(ierror)
   call mmacommget("test", comm, ierror)
   call MPI_COMM_RANK(comm%comm, myrank, ierror)
   write(*,*) comm%rank, myrank
   call mmacommprint(ierror)
      call mmacommregister("test", ierror)
      write(*,*) ierror
   call mmacommfinalize(ierror)
end program

