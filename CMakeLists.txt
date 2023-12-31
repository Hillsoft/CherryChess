﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

message("VERSION=${CMAKE_VERSION}")

if (CMAKE_VERSION VERSION_LESS "3.26")
    # 3.25
    message("Using CMake experimental for 3.25")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
elseif (CMAKE_VERSION VERSION_LESS "3.27")
    # 3.26
    message("Using CMake experimental for 3.26")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif (CMAKE_VERSION VERSION_LESS "3.27.2")
    # 3.27.0 / 3.27.1
    message("Using CMake experimental for 3.27.0 / 3.27.1")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif (CMAKE_VERSION VERSION_LESS "3.28")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "bf70d4b0-9fb7-465c-9803-34014e70d112")
endif()
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)
string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
  "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -E -x c++ <SOURCE>"
  " -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
  " -fmodules-ts -fdep-file=<DYNDEP_FILE> -fdep-output=<OBJECT> -fdep-format=trtbd"
  " -o <PREPROCESSED_SOURCE>")
set(CMAKE_CXX_STANDARD 23)
enable_testing()

#if (MSVC)
  add_compile_options("/Zi")
  add_link_options("/DEBUG")
#endif()

project ("CherryChess")

add_library(stdxx)
if(DEFINED ENV{VCToolsInstallDir})
target_sources(stdxx PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "empty.ixx" )
else()
target_sources(stdxx PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES "std.ixx" )
endif()

# Include sub-projects.
add_subdirectory ("src")
add_subdirectory ("benchmark")
add_subdirectory ("tests")
add_subdirectory ("CherryChess")
