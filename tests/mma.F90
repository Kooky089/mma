#include "mma/mma.fmod"

program main
   use mma
   implicit none
   integer :: ierr
   type(MMAComm), pointer :: comm
   integer :: ierror
   call mmacommregister("test", ierror)
   call mmacommprint(ierror)
   call mmacomminitialize(ierror)
   call mmacommget("test", comm, ierror)
   call mmacommprint(ierror)
   call mmacommregister("test", ierror)
   call mmacommfinalize(ierror)
end program

