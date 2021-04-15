# CMakeLists.txt for the "event" package. It creates a library with a dictionary and a main program.
# If ROOT is not installed in a default system location you need to tell CMake where to find it.
# Sourcing `thisroot.sh` already sets the required environment variables.
# Otherwise, you must tell the build system where to look for ROOT,
# for example by passing `-DROOT_DIR="/path/to/root/installation` at CMake configuration time.

# Set the minimum required version of cmake for a project
  cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
  
# Set the name of the project, and stores it in the variable PROJECT_NAME
  project(roopositron)

# Locate the ROOT package and define a number of useful targets and variables.
  find_package(ROOT REQUIRED COMPONENTS RIO Net)
  
# Create list of ROOT libraries for Cmake
# root-config --glibs

# Get Root libs into cmake variable
# https://stackoverflow.com/questions/13037230/how-do-i-initialize-a-cmake-variable-with-the-result-of-a-shell-command/13037728

# string(REGEX REPLACE <regular_expression>
#       <replacement_expression> <output_variable>
#       <input> [<input>...])

#  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add_definitions("-Wall" "-g")

  message(STATUS "Current directory: ${CMAKE_CURRENT_SOURCE_DIR}")

# Build list of header files and exclude LinkDef from headers
  file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  list(FILTER HEADERS EXCLUDE REGEX "[lL]ink[dD]ef")
  message(STATUS "Found list of headers:")
  message(STATUS "${HEADERS}")

# Find LinkDef.h file
  file(GLOB_RECURSE LINKDEFH ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  list(FILTER LINKDEFH INCLUDE REGEX "[lL]ink[dD]ef")
  message(STATUS "Found LinkDef file: ${LINKDEFH}")
  
# Generate dictionary
  ROOT_GENERATE_DICTIONARY(G__${PROJECT_NAME} ${HEADERS} LINKDEF ${LINKDEFH})
  
# Build list of source files
#  AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
  message(STATUS "Found list of sources:")
  message(STATUS "${SOURCES}")
  
# Create a shared library with a generated dictionary.
  add_library(L__${PROJECT_NAME} ${SOURCES} G__${PROJECT_NAME}.cxx)
  
# Obtain list 
  target_link_libraries(L__${PROJECT_NAME} PUBLIC ROOT::RIO ROOT::Net)

# Find main.cpp
  file(GLOB_RECURSE MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
  list(FILTER MAIN INCLUDE REGEX "main\\.c")
  message(STATUS "Found main file: ${MAIN}")
  
# Create the main program using the library.
  add_executable(Main ${MAIN})
  target_link_libraries(Main L__${PROJECT_NAME})
