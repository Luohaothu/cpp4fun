add_executable(cpp_thread cpp_thread.cpp)
include(FindOpenMP)
target_link_libraries(cpp_thread OpenMP::OpenMP_CXX)