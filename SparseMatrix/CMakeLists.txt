cmake_minimum_required(VERSION 3.9)
project(openmp_project LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(main
    matrix.hpp
    mult.hpp
    helpers.hpp
    helpers.cpp
    main.cpp
)

target_compile_options(main PUBLIC -std=c++20 -Ofast)
target_include_directories(main PUBLIC . SDL2::SDL2)
target_link_libraries(main SDL2::SDL2 OpenMP::OpenMP_CXX)