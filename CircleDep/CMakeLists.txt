add_library(A SHARED A.cpp)
add_library(A_static STATIC A.cpp)
add_library(B  B.cpp)
target_link_libraries(A PRIVATE B)
target_link_libraries(B PUBLIC A_static)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC A)