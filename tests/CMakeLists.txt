cmake_minimum_required(VERSION 2.8)
project(mma-test)


add_executable(test01 test01.c)
target_link_libraries(test01 PRIVATE mma)

if (MSVC)
add_custom_command(
        TARGET test01  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:mma>"
                "$<TARGET_FILE_DIR:test01>/$<TARGET_FILE_NAME:mma>")
endif (MSVC)

add_test(NAME mma_c_single COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test01>)

add_executable(test02 test02.c)
target_link_libraries(test02 PRIVATE mma)

add_executable(test02a test02.c)
target_link_libraries(test02a PRIVATE mma)

add_executable(test02b test02.c)
target_link_libraries(test02b PRIVATE mma)

add_test(NAME mma_c_multi COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test02> a b  : -n 2 $<TARGET_FILE:test02> a c  : -n 2 $<TARGET_FILE:test02a> b d : -n 2 $<TARGET_FILE:test02b> a d)

if(USE_Fortran)
    add_executable(test03 mma.F90)
    target_link_libraries(test03 PUBLIC mma)
    add_test(NAME mma-f-simple COMMAND "${MPIEXEC}" -n 1 $<TARGET_FILE:test03>)
endif()

