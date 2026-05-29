cmake_minimum_required(VERSION 3.25)
project(nomos VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(nomos
    src/nomos.cpp
    src/http.cpp
    src/socket.cpp
)

target_include_directories(nomos PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(nomos_example 
    examples/main.cpp
)

target_link_libraries(nomos_example PRIVATE nomos)
