set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/utf8proc")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/include/utf8proc")

add_library(utf8proc STATIC)
target_sources(
  utf8proc

  PRIVATE
  "${SOURCE_DIR}/utf8proc.c"
  "${INCLUDE_DIR}/utf8proc.h")

target_include_directories(utf8proc PUBLIC "${INCLUDE_DIR}")
target_compile_definitions(utf8proc PUBLIC UTF8PROC_STATIC)
