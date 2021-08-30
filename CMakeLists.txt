cmake_minimum_required(VERSION 3.1)

project(cppjson)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

add_library(cppjson cppjson.cpp)
add_executable(pson_test test.cpp)
target_link_libraries(pson_test cppjson)
