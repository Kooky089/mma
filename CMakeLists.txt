cmake_minimum_required(VERSION 2.8.12)
project(mma C)

include(CheckLanguage)
enable_language (Fortran OPTIONAL)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(CMPI REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)
