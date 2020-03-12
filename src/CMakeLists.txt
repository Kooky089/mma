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
set_target_properties(mma PROPERTIES VERSION ${MMA_VER} SOVERSION ${MMA_SO_VER})

# Generate platform header
include(CheckIncludeFiles)
check_include_files("windows.h" HAVE_WINDOWS_H)
configure_file("mma_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/mma_config.h")

# Generate export header
if(${CMAKE_VERSION} VERSION_LESS "3.12.0")
  message("Workaround for CMAKE < 3.12. Enabling CXX")
  message("See: https://gitlab.kitware.com/cmake/cmake/merge_requests/1799")
  check_language(CXX)
  if(CMAKE_CXX_COMPILER)
    enable_language(CXX)
  else()
    message(FATAL_ERROR "No CXX support")
  endif()
endif()
include(GenerateExportHeader)
generate_export_header(mma)

cmi_set_directory(mma)
