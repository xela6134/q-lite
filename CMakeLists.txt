cmake_minimum_required(VERSION 3.14)
project(qlite VERSION 0.1.0 LANGUAGES CXX)

# Force C++20 (need for std::span, std::variant and stuff)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Performance Flags
# Use best instructions available on current machine
if(MSVC)
    add_compile_options(/W4 /arch:AVX2 /O2)
else()
    add_compile_options(-Wall -Wextra -march=native -O3)
endif()

# Core Engine (database) is separate from CLI
# This does all the calculations, sums, etc
add_library(qlite_engine STATIC
    src/engine.cpp
    src/memory.cpp
)

# Header file location
target_include_directories(qlite_engine PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Executable CLI
add_executable(qlite main.cpp)

# Link executable & engine
target_link_libraries(qlite PRIVATE qlite_engine)

# TODO: Add test support (Google Test)
# TODO: Do benchmarking (Google Benchmark)
