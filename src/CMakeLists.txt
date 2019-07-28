cmake_minimum_required(VERSION 2.8)

cmake_policy(PUSH)
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()


add_library(mma SHARED mma.c string_list.c ../include/mma/mma.h ../include/mma/mma.fmod)
target_include_directories(mma PUBLIC ../include)
if (MSVC)
    target_include_directories(mma PUBLIC ${MPI_INCLUDE})
    target_link_libraries(mma PUBLIC ${MPI_LIB})
endif()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(mma PROPERTIES COMPILE_FLAGS "-ansi -Wall -Wpedantic -Wno-long-long")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
endif()

cmake_policy(POP)