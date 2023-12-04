cmake_minimum_required(VERSION 3.20)
project(earley-algorithm)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(CMAKE_CXX_FLAGS "-w")

add_executable(earley_algorithm src/earley.cpp src/main.cpp)
add_executable(tests  tests/run_tests.cpp tests/tests.cpp)

target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
add_test(unit_test tests)