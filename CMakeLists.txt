cmake_minimum_required(VERSION 3.29)
project(Vulkan)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(Vulkan src/main.cpp)
