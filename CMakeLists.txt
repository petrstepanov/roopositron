# ROOT CMake file written by Petr Stepanov (stepanovps@gmail.com)

# Instructions:
# 1. Put makefile in the program directory containing project files: source files, header files and LinkDef.h
# 2. C++ file containing main() function must be named main.c (or .cc, .cpp etc...)
# 3. Specify the name of your executable below (stored it CMake PROJECT_NAME variable):
  project(roopositron)

# Set the minimum required version of cmake for a project
  cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Locate the ROOT package and define a number of useful targets and variables (need RIO Net?)
  find_package(ROOT REQUIRED COMPONENTS)
# message(STATUS "Found ROOT libraries:")
# message(STATUS "${ROOT_LIBRARIES}")

# Compose list of ROOT libraries with "ROOT::" prefix
  set(LIB_NAMES "")
  FOREACH(X IN LISTS ROOT_LIBRARIES)
    get_filename_component(FILENAME ${X} NAME)
    string(REGEX REPLACE "lib([a-zA-Z0-9]+).so" "ROOT::\\1" FILENAME2 ${FILENAME})
    list(APPEND LIB_NAMES "${FILENAME2}")
  ENDFOREACH()

# Manually append extra ROOT libraries (why missing?)
  list(APPEND LIB_NAMES "ROOT::Gui")
  list(APPEND LIB_NAMES "ROOT::RooFit")
  list(APPEND LIB_NAMES "ROOT::RooFitCore")
  list(APPEND LIB_NAMES "ROOT::Html")
  list(APPEND LIB_NAMES "ROOT::Minuit")
  list(APPEND LIB_NAMES "ROOT::Fumili")

# message(STATUS "Modified ROOT libraries:")
# message(STATUS "${LIB_NAMES}")

# Build list of header files and exclude LinkDef from headers
  file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  list(FILTER HEADERS EXCLUDE REGEX "[lL]ink[dD]ef")
# message(STATUS "Found list of headers:")
# message(STATUS "${HEADERS}")

# Find LinkDef.h file
  file(GLOB_RECURSE LINKDEFH ${CMAKE_CURRENT_SOURCE_DIR}/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  list(FILTER LINKDEFH INCLUDE REGEX "[lL]ink[dD]ef")
# message(STATUS "Found LinkDef file: ${LINKDEFH}")

# Generate dictionary
  ROOT_GENERATE_DICTIONARY(D_${PROJECT_NAME} ${HEADERS} LINKDEF ${LINKDEFH})

# Build list of source files
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
# message(STATUS "Found list of sources:")
# message(STATUS "${SOURCES}")

# Create shared library with a generated dictionary.
  add_library(SO_${PROJECT_NAME} ${SOURCES} D_${PROJECT_NAME}.cxx)

# Link against shared library and list of ROOT libraries
  target_link_libraries(SO_${PROJECT_NAME} PUBLIC ${LIB_NAMES})

# Find location of the enrty point file (main.c*)
  file(GLOB_RECURSE MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
  list(FILTER MAIN INCLUDE REGEX "main\\.c")
# message(STATUS "Found entry point file: ${MAIN}")

# Create the main program using the library.
  add_executable(${PROJECT_NAME} ${MAIN})
  target_link_libraries(${PROJECT_NAME} SO_${PROJECT_NAME})

# Compose the install target
install(TARGETS ${PROJECT_NAME} SO_${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${ROOTSYS}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libD_${PROJECT_NAME}.rootmap
        DESTINATION $ENV{ROOTSYS}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libD_${PROJECT_NAME}_rdict.pcm
        DESTINATION $ENV{ROOTSYS}/lib)
