cmake_minimum_required(VERSION 3.4)

project(parsec)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(COMMON_SRC
    "main.c"
    )

include_directories(
    "."
    )

set(CMAKE_C_STANDARD 11)


add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "-Werror")
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "-Wall")
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "-Wpedantic")

target_link_libraries(${CMAKE_PROJECT_NAME}
    clang
    )
