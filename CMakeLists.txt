cmake_minimum_required(VERSION 3.10)

# Project name
project(base64 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the library
add_library(base64 STATIC
    src/base64.cpp
    include/base64.h
)

# Include directories
target_include_directories(base64 PUBLIC include)

# Add an example executable (optional)
add_executable(base64_example examples/example.cpp)
target_link_libraries(base64_example PRIVATE base64)