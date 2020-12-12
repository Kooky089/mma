#include "mma/mma.fmod"

#define assert(value) if (.not. value) then ; write(*,*) value ; call exit(1) ; end if

program main
   use mma
   implicit none
   type(mma_comm), pointer :: comm
   integer :: ierror
   call mma_comm_register("test ", ierror)
   assert(ierror == 0)
   call mma_print(ierror)
   assert(ierror == 0)
   call mma_initialize(ierror)
   assert(ierror == 0)
   call mma_comm_get("test", comm, ierror)
   assert(ierror == 0)
   call mma_comm_get("test ", comm, ierror)
   assert(ierror == 0)
   call mma_comm_get(" test ", comm, ierror)
   assert(ierror == 0)
   call mma_print(ierror)
   assert(ierror == 0)
   call mma_comm_register("test", ierror)
   assert(ierror /= 0)
   call mma_finalize(ierror)
   assert(ierror == 0)
end program

