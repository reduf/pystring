set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/stb")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/include/stb")

add_library(stb STATIC)

target_sources(
  stb

  PRIVATE
  "${SOURCE_DIR}/stb_sprintf.c"
  "${INCLUDE_DIR}/stb_sprintf.h")

target_include_directories(stb PUBLIC "${INCLUDE_DIR}")
