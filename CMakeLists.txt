cmake_minimum_required(VERSION 3.10)

project(telemetry_bridge C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# Search every .c inside src/
file(GLOB_RECURSE SOURCES
    src/*.c
)

add_executable(telemetry_bridge ${SOURCES})

target_include_directories(telemetry_bridge PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

