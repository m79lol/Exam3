cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

project(hash_server)

project(${PROJECT_NAME} LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(app)
