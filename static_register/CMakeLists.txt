project(static_register)

add_library(lib STATIC lib.cpp)

add_executable(static_register main.cpp)
target_link_libraries(static_register lib)

add_executable(static_register2 main.cpp lib.cpp)