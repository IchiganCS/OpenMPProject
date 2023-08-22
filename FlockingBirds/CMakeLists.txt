cmake_minimum_required(VERSION 3.9)
project(openmp_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE sources 
src/*.cpp 
src/SimpleFlockingBird/*.cpp 
src/render/*.cpp )

add_executable(main
    main.cpp ${sources}
)

target_compile_options(main PUBLIC)
target_include_directories(main PUBLIC . src/ ${SDL2_INCLUDE_DIRS})
# target_include_directories(main PUBLIC .)
target_link_libraries(main OpenMP::OpenMP_CXX ${SDL2_LIBRARIES})
# target_link_libraries(main OpenMP::OpenMP_CXX)