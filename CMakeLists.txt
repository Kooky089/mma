cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)

set(CMI_DOWNLOAD_TAG "47dbfeedab447d1cf94a169e3898d802bebe1c9a")
include(cmi.cmake)

# Optional Fortran
include(CheckLanguage)
enable_language (Fortran OPTIONAL)

cmi_set_build_environment()
cmi_find_mpi()

# Projects
add_subdirectory(src)
add_subdirectory(examples)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

cmi_set_directory(mma)
cmi_set_directory(mma_ex01 IDE "examples")
cmi_set_directory(mma_ex02 IDE "examples")
