set(CMAKE_FOLDER "${CMAKE_FOLDER}/examples")

add_executable(mma_ex01 EXCLUDE_FROM_ALL ex1.c)
target_link_libraries(mma_ex01 PRIVATE mma CMI::MPI_C)

if(TARGET CMI::MPI_F90)
  add_executable(mma_ex02 EXCLUDE_FROM_ALL ex2.F90)
  target_link_libraries(mma_ex02 PRIVATE mma CMI::MPI_F90)
endif()
