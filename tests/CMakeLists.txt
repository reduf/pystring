add_executable(tests main.cpp pystr.cpp)
target_link_libraries(tests PRIVATE pystr)

add_test(NAME tests COMMAND tests)
