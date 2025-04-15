cmake_minimum_required(VERSION 3.26)

project(TesTriste LANGUAGES C CXX VERSION 0.1)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/helper.cmake")

set(default_build_type "RelWithDebInfo")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
   set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS  "Debug" "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -format-style='file';
  -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(BUILD_SHARED_LIBS ON)

add_subdirectory(src)