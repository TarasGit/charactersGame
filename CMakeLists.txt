cmake_minimum_required(VERSION 3.5)

project(trinamixGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Threads)

add_executable(charactersGame main.cpp)
target_link_libraries (charactersGame ${CMAKE_THREAD_LIBS_INIT})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  game_tests
  game_tests.cpp
)
target_link_libraries(
  game_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(game_tests)

