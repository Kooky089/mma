cmake_minimum_required(VERSION 2.8)
project(mma-examples)

add_executable(ex1 ex1.c)
target_link_libraries(ex1 PRIVATE mma)

if (USE_Fortran)
    add_executable(ex2 ex2.F90)
    target_link_libraries(ex2 PUBLIC mma)
endif()
