cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)

set(CMI_DOWNLOAD_TAG "5346dc2806e39ff86d53a28406c49d7693505dac")
include(cmi.cmake)

enable_testing()

# Optional Fortran
include(CheckLanguage)
enable_language (Fortran OPTIONAL)

cmi_set_build_environment()
cmi_find_mpi()

# Projects
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)

cmi_set_directory(mma)
cmi_set_directory(mma_ex01 IDE "examples")
cmi_set_directory(mma_ex02 IDE "examples")
cmi_set_directory(mma_test01 IDE "tests")
cmi_set_directory(mma_test02 IDE "tests")
cmi_set_directory(mma_test02a IDE "tests")
cmi_set_directory(mma_test02b IDE "tests")
cmi_set_directory(mma_test03 IDE "tests")
