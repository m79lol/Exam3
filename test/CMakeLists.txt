project(hash_server_tests)

###########################
# Targets
###########################
add_executable(${PROJECT_NAME} 
  "src/test.cpp"
)

###########################
# Properties
###########################
set_target_properties(${PROJECT_NAME} PROPERTIES
  DEBUG_POSTFIX "_debug"
)

###########################
# Libraries
###########################
target_link_libraries(${PROJECT_NAME}
  sub::lib
)

###########################
# Tests
###########################
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

###########################
# Install
###########################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)