cmake_minimum_required(VERSION 2.6)
project(threadpool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(threadpool threadpool.cpp main.cpp)

install(TARGETS threadpool RUNTIME DESTINATION bin)
