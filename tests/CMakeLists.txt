add_executable(mma_test01 test01.c)
target_link_libraries(mma_test01 PRIVATE mma)
cmi_set_directory(mma_test01 IDE "tests")
add_test(NAME mma.c_single COMMAND ${CMI_MPIEXEC} -n 1 $<TARGET_FILE:mma_test01>)

add_executable(mma_test02 test02.c)
target_link_libraries(mma_test02 PRIVATE mma)
cmi_set_directory(mma_test02 IDE "tests")

add_executable(mma_test02a test02.c)
target_link_libraries(mma_test02a PRIVATE mma)
cmi_set_directory(mma_test02a IDE "tests")

add_executable(mma_test02b test02.c)
target_link_libraries(mma_test02b PRIVATE mma)
cmi_set_directory(mma_test02b IDE "tests")

add_test(
  NAME mma.c_multi
  COMMAND ${CMI_MPIEXEC} -n 3 $<TARGET_FILE:mma_test02> : -n 2 $<TARGET_FILE:mma_test02a> : -n 2 $<TARGET_FILE:mma_test02b>)

if(CMAKE_Fortran_COMPILER)
  add_executable(mma_test03 mma.F90)
  target_link_libraries(mma_test03 PUBLIC mma)
  add_test(NAME mma.f-simple COMMAND ${CMI_MPIEXEC} -n 1 $<TARGET_FILE:mma_test03>)
  cmi_set_directory(mma_test03 IDE "tests")
endif()

