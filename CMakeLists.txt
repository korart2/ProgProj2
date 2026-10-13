cmake_minimum_required(VERSION 3.25)
project(ProgrammingProject2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ProgrammingProject2
    src/main.cpp
    src/sequence_io.cpp
    src/experiments.cpp
    src/parallel_dot_product.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(ProgrammingProject2 PRIVATE Threads::Threads)

if(MSVC)
    target_compile_options(ProgrammingProject2 PRIVATE /EHsc /W4)
endif()