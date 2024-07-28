

cmake_minimum_required(VERSION 3.22)

project(examples CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(readelf read_elf.cpp)
target_link_libraries(readelf PRIVATE riscv_elf)
