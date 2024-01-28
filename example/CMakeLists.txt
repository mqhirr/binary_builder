cmake_minimum_required(VERSION 3.20)

project(example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MQH_BINARY_BUILDER_SUPPORT_STL 1)

add_executable(example main.cpp)
target_link_libraries(example mqh::binary_builder)