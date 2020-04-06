cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)

set(CMI_DOWNLOAD_TAG 34e7114e7d964d224ab0227cb8dabed6cd024d4d)
include(cmi.cmake)

# Optional Fortran
include(CheckLanguage)
enable_language(Fortran)

cmi_set_build_environment()
cmi_find_mpi(COMPONENTS C)

# Projects
add_subdirectory(src)
add_subdirectory(examples)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
