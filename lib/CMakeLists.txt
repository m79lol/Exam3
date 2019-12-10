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
