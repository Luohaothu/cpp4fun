cmake_minimum_required(VERSION 3.25)

project(cxx_module CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -fmodules-ts)
add_library(hello STATIC hello.cpp)
add_executable(cxx_module main.cpp)
target_link_libraries(cxx_module PUBLIC hello)