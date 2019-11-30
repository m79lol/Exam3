cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
set(PROJECT hasher)
project(${PROJECT} LANGUAGES CXX)

#################################################################################
# Packages
#################################################################################

#set (Boost_DEBUG FALSE)
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_LIBS FALSE)
find_package(Boost 1.66.0 REQUIRED system)

#################################################################################
# Sources
#################################################################################
set(SOURCES
  "src/hash_server.cpp"
)

include_directories(
  ${Boost_INCLUDE_DIRS}
  "PicoSHA2"
  "include"
)

link_directories(${Boost_LIBRARY_DIRS})

add_library(${PROJECT} SHARED ${SOURCES})

#################################################################################
# Properties
#################################################################################
target_compile_definitions(${PROJECT}
  PRIVATE DLL_API_EXPORT
)

set_target_properties(${PROJECT} PROPERTIES
  PREFIX "lib"
  DEBUG_POSTFIX "_debug"
)

#################################################################################
# Libraries
#################################################################################

target_link_libraries(${PROJECT} ${Boost_LIBRARIES})

#################################################################################
# Install
#################################################################################

install(TARGETS ${PROJECT} EXPORT ${PROJECT} DESTINATION bin)
install(EXPORT ${PROJECT} DESTINATION config)
install(DIRECTORY ${CMAKE_HOME_DIRECTORY}/include/ DESTINATION include)
