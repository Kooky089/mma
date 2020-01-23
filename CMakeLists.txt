cmake_minimum_required(VERSION 3.8)
set(MMA_VER 1.0.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)
enable_testing()

set(cmi_DOWNLOAD_TAG "73f93afce78cb9d6fcb8cefc212ac482d73378e7")
include(cmiLoader.cmake)
cmi_add_archive_url(cmi
  "https://gitlab.com/nordfox/cmakeit/-/archive/${cmi_DOWNLOAD_TAG}/cmakeit-${cmi_DOWNLOAD_TAG}.tar.gz"
)
include(cmiBuild)

# Optional Fortran
include(CheckLanguage)
check_language(Fortran)
if(CMAKE_Fortran_COMPILER)
  enable_language(Fortran)
else()
  message(STATUS "No Fortran support")
endif()

find_package(CMPI REQUIRED)

# Compiler flags
include(cmiCompilerFlags)
cmi_disable_vs_debug_runtime()

# Projects
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)

cmi_set_output_directory(mma "${cmi_BIN_DIR}" "")
cmi_set_output_directory(mma_ex01 "${cmi_BIN_DIR}" "/examples")
cmi_set_output_directory(mma_ex02 "${cmi_BIN_DIR}" "/examples")
cmi_set_output_directory(mma_test01 "${cmi_BIN_DIR}" "/tests")
cmi_set_output_directory(mma_test02 "${cmi_BIN_DIR}" "/tests")
cmi_set_output_directory(mma_test02a "${cmi_BIN_DIR}" "/tests")
cmi_set_output_directory(mma_test02b "${cmi_BIN_DIR}" "/tests")
cmi_set_output_directory(mma_test03 "${cmi_BIN_DIR}" "/tests")

