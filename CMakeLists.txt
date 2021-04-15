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
#  find_package(ROOT REQUIRED COMPONENTS RIO Net)
  find_package(ROOT REQUIRED COMPONENTS)
  
  message(STATUS "Found ROOT libraries:")
  message(STATUS "${ROOT_LIBRARIES}")
  
  #set(LIBS ${ROOT_LIBRARIES})
# https://stackoverflow.com/questions/4346412/how-to-prepend-all-filenames-on-the-list-with-common-path/27630120
  get_filename_component(LIB_NAMES "/petrstepanov/Applications/root-6.22.08/lib/libTreePlayer.so" NAME)
  # cmake_path(GET ${ROOT_LIBRARIES} FILENAME LIB_NAMES)
  # string(REGEX REPLACE "lib(\\w+).so" "$1" LIB_NAMES ${ROOT_LIBRARIES})
  
  set(LIB_NAMES "")
  
  FOREACH(X IN LISTS ROOT_LIBRARIES)
    get_filename_component(FILENAME ${X} NAME)
    string(REGEX REPLACE "lib([a-zA-Z0-9]+).so" "ROOT::\\1" FILENAME2 ${FILENAME})
    # message(STATUS "${FILENAME}")
    # message(STATUS "${FILENAME2}")
    list(APPEND LIB_NAMES "${FILENAME2}")
  ENDFOREACH()
  
# Manually add rest of libraries
  list(APPEND LIB_NAMES "ROOT::Gui") # optional
  list(APPEND LIB_NAMES "ROOT::RooFit")
  list(APPEND LIB_NAMES "ROOT::RooFitCore")
  list(APPEND LIB_NAMES "ROOT::Html")
  list(APPEND LIB_NAMES "ROOT::Minuit")
  list(APPEND LIB_NAMES "ROOT::Fumili")

# Print ROOT libraries
  message(STATUS "Modified ROOT libraries:")
  message(STATUS "${LIB_NAMES}")
  
  
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
  add_library(SO_${PROJECT_NAME} ${SOURCES} G__${PROJECT_NAME}.cxx)
  
# Obtain list 
  target_link_libraries(SO_${PROJECT_NAME} PUBLIC ${LIB_NAMES})

# Find main.cpp
  file(GLOB_RECURSE MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
  list(FILTER MAIN INCLUDE REGEX "main\\.c")
  message(STATUS "Found main file: ${MAIN}")
  
# Create the main program using the library.
  add_executable(${PROJECT_NAME} ${MAIN})
  target_link_libraries(${PROJECT_NAME} SO_${PROJECT_NAME})
