# tests/CMakeLists.txt

add_executable(unit_tests
    unit/test_core.cpp
    unit/test_ops.cpp
    unit/test_engine.cpp
)

target_link_libraries(unit_tests 
    PRIVATE 
    gtest_main
    qlite_lib
)

target_compile_features(unit_tests PUBLIC cxx_std_20)

include(GoogleTest)
gtest_discover_tests(unit_tests)
