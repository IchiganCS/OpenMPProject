cmake_minimum_required(VERSION 3.9)
project(openmp_project LANGUAGES CXX)

find_package(OpenMP REQUIRED)

add_executable(main
    matrix.hpp
    mult.hpp
    csv.hpp
    csv.cpp
    helpers.hpp
    helpers.cpp
    main.cpp
)

target_compile_options(main PUBLIC -std=c++20 -Ofast)
target_include_directories(main PUBLIC .)
target_link_libraries(main OpenMP::OpenMP_CXX)