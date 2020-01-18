add_executable(mma_ex01 ex1.c)
target_link_libraries(mma_ex01 PRIVATE mma)

if (MPI_Fortran_FOUND)
    add_executable(mma_ex02 ex2.F90)
    target_link_libraries(mma_ex02 PUBLIC mma)
endif()
