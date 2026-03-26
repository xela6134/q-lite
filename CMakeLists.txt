cmake_minimum_required(VERSION 3.14)
project(qlite VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tests
enable_testing()

include(FetchContent)

# Test Setup
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Only force shared CRT if using Visual Studio (MSVC)
if(MSVC)
    add_compile_options(/O2 /arch:AVX2)
else()
    add_compile_options(-O3 -march=native -Wall -Wextra)
endif()

FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(tests)
