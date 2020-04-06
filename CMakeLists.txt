cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)

set(CMI_DOWNLOAD_TAG 364b54e0200459f446190a94d177d27317d9447b)
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
