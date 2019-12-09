# On Linux: User must provide MPI wrapper. This is the only way to get Fortran MPI modules work right, when switching between ifort and gfortran.
# On Windows: Since MPI wrapper are not available/broken, we rely on cmake's findMPI
# Usage (Windows/Linux):
# target_include_directories(mytarget PUBLIC "${MPI_INCLUDE}")
# target_link_libraries(mytarget PUBLIC "${MPI_LIB}")
# Setting up MPI
cmake_policy(PUSH)
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
if (MSVC)
  # Don't use FindMPI when using MS Visual Studio
  set (MPI_FOUND FALSE)
  if (NOT DEFINED I_MPI_ROOT)
    set (CMPI_ROOT "$ENV{I_MPI_ROOT}")
  else ()
    set (CMPI_ROOT "${I_MPI_ROOT}")
  endif ()
  file(TO_CMAKE_PATH "${CMPI_ROOT}" CMPI_ROOT)
  
  if (NOT EXISTS "${CMPI_ROOT}") 
    message (STATUS " Folder does NOT exist: I_MPI_ROOT=${CMPI_ROOT}")
  else ()
    set (MPI_INCLUDE "${CMPI_ROOT}/intel64/include")
    set (MPI_LIB "${CMPI_ROOT}/intel64/lib/release/impi.lib")
    set (MPIEXEC "${CMPI_ROOT}/intel64/bin/mpiexec.exe")
    message (STATUS " I_MPI_ROOT: " "${CMPI_ROOT}")
    
    if (NOT EXISTS "${MPI_INCLUDE}")
      message (STATUS " MPI_INCLUDE path does NOT exist: ${MPI_INCLUDE}")
    else ()
      message (STATUS " MPI_INCLUDE: ${MPI_INCLUDE}")
      
      if (NOT EXISTS "${MPI_LIB}")
        message (STATUS " MPI_LIB path does not exist: ${MPI_LIB}")
      else ()
        message (STATUS " MPI_LIB: ${MPI_LIB}")
        
        if (NOT EXISTS "${MPIEXEC}")
          message (STATUS " MPIEXEC does not exist: ${MPIEXEC}")
        else ()
          message (STATUS " MPIEXEC: ${MPIEXEC}")
          
          # Check C and CXX
          if (CMAKE_C_COMPILER OR CMAKE_CXX_COMPILER)
            if (NOT EXISTS "${MPI_INCLUDE}/mpi.h")
              message (STATUS " Could NOT find mpi.h file in: ${MPI_INCLUDE}")
            else ()
              if (CMAKE_C_COMPILER)
                #try_run(MPI_C_RUNS
                #  MPI_C_COMPILES
                #  ${CMAKE_CURRENT_BINARY_DIR}/cmpi
                #  "${CMAKE_CURRENT_LIST_DIR}/mpi_test.c"
                #  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${MPI_INCLUDE}"
                #  LINK_LIBRARIES ${MPI_LIB}
                #  )
                #if(MPI_C_COMPILES)
                  set (MPI_FOUND TRUE)
                  set (MPI_C_FOUND TRUE)
                #  if(MPI_C_RUNS STREQUAL "0")
                #    set (MPI_C_WORKS TRUE)
                #  else()
                #    message(WARNING " Could NOT run MPI sample. Either MPI Runtime is missing or not loaded correctly into PATH")
                #  endif()
                #else()
                #  message(STATUS " Could NOT compile MPI sample")
                #endif()
              endif ()
              if (CMAKE_CXX_COMPILER)
                #try_run(MPI_CXX_RUNS
                #  MPI_CXX_COMPILES
                #  ${CMAKE_CURRENT_BINARY_DIR}/cmpi
                #  "${CMAKE_CURRENT_LIST_DIR}/mpi_test.cpp"
                #  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${MPI_INCLUDE}"
                #  LINK_LIBRARIES ${MPI_LIB}
                #  )
                #if(MPI_CXX_COMPILES)
                  set (MPI_FOUND TRUE)
                  set (MPI_CXX_FOUND TRUE)
                #  if(MPI_CXX_RUNS STREQUAL "0")
                #    set (MPI_CXX_WORKS TRUE)
                #  else()
                #    message(WARNING " Could NOT run MPI sample. Either MPI Runtime is missing or not loaded correctly into PATH")
                #  endif()
                #else()
                #  message(STATUS " Could NOT compile MPI sample")
                #endif()
              endif ()
            endif ()
          endif ()
          
          # Check Fortran
          if (CMAKE_Fortran_COMPILER)
            if (NOT EXISTS "${MPI_INCLUDE}/mpif.h")
              message (STATUS " Could NOT find mpif.h file in: ${MPI_INCLUDE}")
            else ()
              if (CMAKE_Fortran_COMPILER)
                set (MPI_FOUND TRUE)
                set (MPI_Fortran_FOUND TRUE)
              endif ()
            endif ()
          endif ()
          
        endif ()
      endif ()
    endif ()
  endif ()
  unset (CMPI_ROOT)
  if (NOT MPI_FOUND)
    message(WARNING " Make sure that IntelMPI SDK is installed and I_MPI_ROOT points to it. For example:\n"
                    " I_MPI_ROOT=C:\\Program Files (x86)\\IntelSWTools\\compilers_and_libraries_2019.5.281\\windows\\mpi\\\n")
  endif ()
else ()
  set (MPI_C_COMPILER ${CMAKE_C_COMPILER})
  set (MPI_CXX_COMPILER ${CMAKE_CXX_COMPILER})
  set (MPI_Fortran_COMPILER ${CMAKE_Fortran_COMPILER})
  find_package (MPI)
  if (CMAKE_C_COMPILER)
    if (NOT MPI_C_FOUND)
      message (STATUS "C compiler ${CMAKE_C_COMPILER} has NO MPI support.\nTo enable support, provide MPI compiler wrapper explicitly e.g. \"CC=mpicc cmake\", delete cache and rerun cmake.\n")
    endif ()
  endif ()
  if (CMAKE_CXX_COMPILER)
    if (NOT MPI_CXX_FOUND)
      message (STATUS "CXX compiler ${CMAKE_CXX_COMPILER} has NO MPI support.\nTo enable support, provide MPI compiler wrapper explicitly e.g. \"CXX=mpicxx cmake\", delete cache and rerun cmake.\n")
    endif ()
  endif ()
  if (CMAKE_Fortran_COMPILER)
    if (NOT MPI_Fortran_FOUND)
      message (STATUS "Fortran compiler ${CMAKE_Fortran_COMPILER} has NO MPI support.\nTo enable support, provide MPI compiler warpper explicitly e.g. \"FC=mpifc cmake\", delete cache and rerun cmake.\n")
    endif ()
  endif ()
  set (MPI_INCLUDE "")
  set (MPI_LIB "")
endif ()

foreach (_comp IN ITEMS C CXX Fortran)
  set (CMPI_${_comp}_FOUND ${MPI_${_comp}_FOUND})
  
  if (CMPI_FIND_REQUIRED)
    if (CMAKE_${_comp}_COMPILER AND NOT CMPI_${_comp}_FOUND)
      set (MPI_FOUND FALSE)
    endif ()
  endif ()
endforeach ()
unset (_comp)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(CMPI REQUIRED_VARS MPI_FOUND HANDLE_COMPONENTS)

cmake_policy(POP)
