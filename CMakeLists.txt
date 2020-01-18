cmake_minimum_required(VERSION 3.8)
set(MMA_VER 0.1.0)
set(MMA_SO_VER 1)
project(mma VERSION ${MMA_VER} LANGUAGES C)
enable_testing()

set(cmi_DOWNLOAD_TAG "95a141e3d245d22de463bab7a516c0ff456b29f9")
include(cmiLoader.cmake)
cmi_add_archive_url(cmi
  "https://gitlab.com/nordfox/cmakeit/-/archive/${cmi_DOWNLOAD_TAG}/cmakeit-${cmi_DOWNLOAD_TAG}.tar.gz"
)


# Optional Fortran
include(CheckLanguage)
check_language(Fortran)
if(CMAKE_Fortran_COMPILER)
  enable_language(Fortran)
else()
  message(STATUS "No Fortran support")
endif()

find_package(CMPI REQUIRED)

## Put projects into folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/package/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/package/$<CONFIG>/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/package/$<CONFIG>/bin)

# Compiler flags
include(cmiCompilerFlags)
cmi_disable_vs_debug_runtime()

# Projects
add_subdirectory(src)
add_subdirectory(examples)

add_subdirectory(tests)

# IDE settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_presets")
set_target_properties(mma PROPERTIES FOLDER "${PROJECT_NAME}")
set_target_properties(mma_ex01 PROPERTIES FOLDER "${PROJECT_NAME}/examples")
set_target_properties(mma_ex02 PROPERTIES FOLDER "${PROJECT_NAME}/examples")
set_target_properties(mma_test01 PROPERTIES FOLDER "${PROJECT_NAME}/tests")
set_target_properties(mma_test02 PROPERTIES FOLDER "${PROJECT_NAME}/tests")
set_target_properties(mma_test03 PROPERTIES FOLDER "${PROJECT_NAME}/tests")

