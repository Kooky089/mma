cmake_minimum_required(VERSION 2.8)
project(mma-apps)

add_executable(mma-app-c mma.c)
target_link_libraries(mma-app-c PRIVATE mma)

if (USE_Fortran)
    add_executable(mma-app-f mma.F90)
    target_link_libraries(mma-app-f PUBLIC mma)
endif()
