set(CMAKE_FOLDER "${CMAKE_FOLDER}/tests")

add_executable(mma_test01 test01.c)
target_link_libraries(mma_test01 PRIVATE mma)
add_test(NAME mma.c_single COMMAND ${CMI_MPIEXEC} $ENV{MMA_MPI_RUN_FLAG} -n 1 $<TARGET_FILE:mma_test01>)

add_executable(mma_test02a test02.c)
target_link_libraries(mma_test02a PRIVATE mma)

add_executable(mma_test02b test02.c)
target_link_libraries(mma_test02b PRIVATE mma)

add_test(
  NAME mma.c_multi
  COMMAND ${CMI_MPIEXEC} $ENV{MMA_MPI_RUN_FLAG} -n 3 $<TARGET_FILE:mma_test02a> : -n 4 $<TARGET_FILE:mma_test02b>)

if(CMAKE_Fortran_COMPILER)
  add_executable(mma_test03 mma.F90)
  target_link_libraries(mma_test03 PUBLIC mma)
  add_test(NAME mma.f-simple COMMAND ${CMI_MPIEXEC} $ENV{MMA_MPI_RUN_FLAG} -n 1 $<TARGET_FILE:mma_test03>)
endif()

