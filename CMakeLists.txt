cmake_minimum_required(VERSION 3.8)
set(MMA_VERSION 1.0.0)
set(MMA_PROTOCOL_VERSION 1)
project(mma VERSION ${MMA_VERSION} LANGUAGES C)

set(CMI_DOWNLOAD_TAG 88db46577abfbf624de26c300eb04b741d036493)
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
