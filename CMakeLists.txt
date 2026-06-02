cmake_minimum_required(VERSION 3.25)
project(nomos VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # important for clangd!!

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core Library
add_library(nomos
    src/nomos.cpp
    src/http.cpp
    src/socket.cpp
    src/thread_pool.cpp
)

target_include_directories(nomos PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(nomos_example 
    examples/main.cpp
)

target_link_libraries(nomos_example PRIVATE nomos)

enable_testing() 

include(FetchContent)

set(CATCH_BUILD_TESTING OFF CACHE INTERNAL "")
set(CATCH_BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 
)
FetchContent_MakeAvailable(Catch2)

add_executable(nomos_tests
    tests/test_http.cpp
)

target_link_libraries(nomos_tests PRIVATE nomos Catch2::Catch2WithMain)

add_test(NAME HttpTests COMMAND nomos_tests)
