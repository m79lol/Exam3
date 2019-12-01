cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(hash_server)

###########################
# Config
###########################
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_STANDARD 14)
project(${PROJECT_NAME} LANGUAGES CXX)
enable_testing()

###########################
# Packages
###########################
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
find_package(Boost 1.66.0 REQUIRED system unit_test_framework)

###########################
# Subprojects
###########################
add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(test)
