add_executable(mma_test01 test01.c)
target_link_libraries(mma_test01 PRIVATE mma)

add_test(NAME mma.c_single COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:mma_test01>)

add_executable(mma_test02 test02.c)
target_link_libraries(mma_test02 PRIVATE mma)

add_executable(mma_test02a test02.c)
target_link_libraries(mma_test02a PRIVATE mma)

add_executable(mma_test02b test02.c)
target_link_libraries(mma_test02b PRIVATE mma)

add_test(
  NAME mma.c_multi
  COMMAND cmi::MPIEXEC -n 3 $<TARGET_FILE:mma_test02> : -n 2 $<TARGET_FILE:mma_test02a> : -n 2 $<TARGET_FILE:mma_test02b>)

cmi_set_directory(mma_test01 IDE "tests")
cmi_set_directory(mma_test02 IDE "tests")
cmi_set_directory(mma_test02a IDE "tests")
cmi_set_directory(mma_test02b IDE "tests")


if(CMAKE_Fortran_COMPILER)
  add_executable(mma_test03 mma.F90)
  target_link_libraries(mma_test03 PUBLIC mma)
  add_test(NAME mma.f-simple COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:mma_test03>)
  cmi_set_directory(mma_test03 IDE "tests")
endif()

