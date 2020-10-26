cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)

set(CMI_DOWNLOAD_TAG 9830c79124380fa4958a61f37807948dc48961dc)
include(cmi.cmake)

# Optional Fortran
include(CheckLanguage)
enable_language(Fortran)

cmi_set_build_environment()
cmi_find_mpi(REQUIRED COMPONENTS C)
cmi_find_mpi(COMPONENTS F90)

# Projects
add_subdirectory(src)
add_subdirectory(examples)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
