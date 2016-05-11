cmake_minimum_required (VERSION 2.6)
project (Spriter2Moai)
# The version number.
set (Spriter2Moai_VERSION_MAJOR 1)
set (Spriter2Moai_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Spriter2MoaiConfig.h.in"
  "${PROJECT_BINARY_DIR}/Spriter2MoaiConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(Spriter2Moai main.cxx)
