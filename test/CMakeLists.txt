


cmake_minimum_required(VERSION 3.22)

project(examples CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-fsanitize=leak -fsanitize=address")

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE riscv_elf)
