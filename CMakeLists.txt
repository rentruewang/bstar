cmake_minimum_required(VERSION 3.22)
project(bstar)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)

file(GLOB FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(bstar ${FILES})
target_include_directories(bstar PRIVATE "include")
