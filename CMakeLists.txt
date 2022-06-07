cmake_minimum_required(VERSION 3.13)
project(cpp4fun)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cpp_thread)

add_subdirectory(meta_programming)

add_subdirectory(Catch2Frame)

add_subdirectory(static_dispatch)

add_subdirectory(huawei)

add_subdirectory(CircleDep)