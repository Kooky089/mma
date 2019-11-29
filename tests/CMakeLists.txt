add_executable(test01 test01.c)
target_link_libraries(test01 PRIVATE mma)

if(WIN32)
  add_custom_command(
          TARGET test01 POST_BUILD
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                  "$<TARGET_FILE:mma>"
                  "$<TARGET_FILE_DIR:test01>/$<TARGET_FILE_NAME:mma>")
endif()

add_test(NAME mma.c_single COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test01>)

add_executable(test02 test02.c)
target_link_libraries(test02 PRIVATE mma)

add_executable(test02a test02.c)
target_link_libraries(test02a PRIVATE mma)

add_executable(test02b test02.c)
target_link_libraries(test02b PRIVATE mma)

add_test(NAME mma.c_multi COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test02> : -n 2 $<TARGET_FILE:test02> : -n 2 $<TARGET_FILE:test02a> : -n 2 $<TARGET_FILE:test02b>)

if(CMAKE_Fortran_COMPILER)
  add_executable(test03 mma.F90)
  target_link_libraries(test03 PUBLIC mma)
  add_test(NAME mma.f-simple COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test03>)
endif()
