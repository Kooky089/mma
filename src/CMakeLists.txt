set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(mma SHARED
  mma.c
  string_list.c
  ../include/mma/mma.h
  ../include/mma/mma.fmod
)

target_include_directories(mma PUBLIC "../include")
target_include_directories(mma PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(mma PUBLIC "${MPI_INCLUDE}")
target_link_libraries(mma PUBLIC "${MPI_LIB}")

include(GenerateExportHeader)
generate_export_header(mma)
