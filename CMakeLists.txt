

cmake_minimum_required(VERSION 3.22)

project(riscv-elf-lib CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

add_library(riscv_elf SHARED src/riscv_elf.cpp)
target_include_directories(riscv_elf PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
)

add_subdirectory(example/)
add_subdirectory(test/)
