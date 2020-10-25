cmake_minimum_required(VERSION 3.1)

project(
  pystr
  VERSION 1.0
  LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include_directories("deps/include")

include(stb)
include(utf8proc)

add_library(pystr pystr.h pystr.cpp)
target_include_directories(pystr PUBLIC "${CMAKE_SOURCE_DIRECTORY}")
target_link_libraries(pystr PRIVATE stb utf8proc)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
