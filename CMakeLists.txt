cmake_minimum_required(VERSION "3.20")

project(dns-client)

include_directories(
        includes
)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_executable(${PROJECT_NAME}
        ${SRC_FILES})

