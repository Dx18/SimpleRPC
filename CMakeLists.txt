cmake_minimum_required(VERSION 3.0)

project(SimpleRPC C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

include_directories(include)

add_subdirectory(util)
add_subdirectory(simple_rpc)

include(cmake/SimpleRPCGenerator.cmake)

add_subdirectory(examples)
