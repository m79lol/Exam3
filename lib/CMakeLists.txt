project(HashServer)

###########################
# Targets
###########################
add_library(${PROJECT_NAME} SHARED 
  "src/HashServer.cpp"
  "src/HashServerImp.cpp"
)
add_library(sub::lib ALIAS ${PROJECT_NAME})

###########################
# Properties
###########################
set_target_properties(${PROJECT_NAME} PROPERTIES
  DEBUG_POSTFIX "_debug"
)
if (NOT MSVC)
  find_package(Threads REQUIRED)
  if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
  endif()
  if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
  endif()

  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH ".")
endif()

###########################
# Includes
###########################
target_include_directories(${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/PicoSHA2>
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  PUBLIC $<INSTALL_INTERFACE:include>
)

###########################
# Libraries
###########################
target_link_directories(${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${Boost_LIBRARY_DIRS}>
)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

###########################
# Install
###########################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)
install(EXPORT ${PROJECT_NAME} DESTINATION config)
install(DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib/include/ DESTINATION include)
