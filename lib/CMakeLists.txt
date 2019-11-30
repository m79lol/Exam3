project(libhash_server)

# Packages
set(Boost_USE_MULTITHREADED TRUE)
set(Boost_USE_STATIC_LIBS FALSE)
find_package(Boost 1.66.0 REQUIRED system)

###########################
# Targets
###########################
add_library(${PROJECT_NAME} SHARED 
  "src/lib.cpp"
)
add_library(sub::lib ALIAS ${PROJECT_NAME})

###########################
# Properties
###########################
target_compile_definitions(${PROJECT_NAME}
  PRIVATE DLL_API_EXPORT
)
set_target_properties(${PROJECT} PROPERTIES
  DEBUG_POSTFIX "_debug"
)

###########################
# Includes
###########################
target_include_directories(${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/PicoSHA2>
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
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