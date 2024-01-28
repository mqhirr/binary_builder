cmake_minimum_required(VERSION 3.20)

project(mqh::binary_builder)

option(MQH_BINARY_BUILDER_SUPPORT_STL "support some stl types?" 0)

add_compile_definitions(MQH_BINARY_BUILDER_SUPPORT_STL=${MQH_BINARY_BUILDER_SUPPORT_STL})

add_library(mqh_binary_builder INTERFACE)
add_library(mqh::binary_builder ALIAS mqh_binary_builder)
target_include_directories(mqh_binary_builder INTERFACE include)

add_subdirectory(example)