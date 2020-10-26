set(CMAKE_FOLDER "${CMAKE_FOLDER}/examples")

add_executable(mma_ex01 ex1.c)
target_link_libraries(mma_ex01 PRIVATE mma CMI::MPI_C)

if(TARGET CMI::MPI_F90)
  add_executable(mma_ex02 ex2.F90)
  target_link_libraries(mma_ex02 PRIVATE mma CMI::MPI_F90)
endif()
