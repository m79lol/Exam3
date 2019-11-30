cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(hash_server)

project(${PROJECT_NAME} LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(app)