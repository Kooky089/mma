cmake_minimum_required(VERSION 3.0)
set(MMA_VER 0.1.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)
enable_testing()

include(cmi.cmake)

# Optional Fortran
include(CheckLanguage)
check_language(Fortran)
if(CMAKE_Fortran_COMPILER)
  enable_language(Fortran)
else()
  message(STATUS "No Fortran support")
endif()

find_package(CMPI REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)

add_subdirectory(tests)
