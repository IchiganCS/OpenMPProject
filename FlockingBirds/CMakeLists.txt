cmake_minimum_required(VERSION 3.9)
project(openmp_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB sources ./*.cpp ./*.h)

add_executable(main
    ${sources}
)

target_compile_options(main PUBLIC)
target_include_directories(main PUBLIC . SDL2::SDL2)
target_link_libraries(main OpenMP::OpenMP_CXX SDL2::SDL2)