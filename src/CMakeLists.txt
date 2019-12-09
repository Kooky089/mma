set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(mma SHARED
  mma.c
  string_list.c
  string_list.h
  ../include/mma/mma.h
  ../include/mma/mma.fmod
)

target_include_directories(mma PUBLIC "../include")
target_include_directories(mma PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(mma PUBLIC "${MPI_INCLUDE}")
target_link_libraries(mma PUBLIC "${MPI_LIB}")

include(CheckIncludeFiles)
check_include_files("windows.h" HAVE_WINDOWS_H)

configure_file("mma_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/mma_config.h")

include(GenerateExportHeader)
generate_export_header(mma)
