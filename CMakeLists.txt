cmake_minimum_required(VERSION 3.5)

project(log_rotate)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_subdirectory(src/lib/ulog)
add_subdirectory(src/lib/CLI11)

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME} pthread)

target_link_libraries(${PROJECT_NAME} ulog)
target_link_libraries(${PROJECT_NAME} CLI11)
