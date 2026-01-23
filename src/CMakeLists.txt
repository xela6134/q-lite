# qlite/src/CMakeLists.txt
add_library(qlite_core
    core/object.cpp
    ops/math.cpp
)

target_include_directories(qlite_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(qlite main.cpp)

target_link_libraries(qlite PRIVATE qlite_core)
