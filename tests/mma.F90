#include "mma/mma.fmod"

program main
   use mma
   implicit none
   type(mma_comm), pointer :: comm
   integer :: ierror
   call mma_comm_register("test", ierror)
   call mma_print(ierror)
   call mma_initialize(ierror)
   call mma_comm_get("test", comm, ierror)
   call mma_print(ierror)
   call mma_comm_register("test", ierror)
   call mma_finalize(ierror)
end program

