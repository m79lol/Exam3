project(hash_server_app)

###########################
# Targets
###########################
add_executable(${PROJECT_NAME} 
  "src/app.cpp"
)

###########################
# Properties
###########################
set_target_properties(${PROJECT} PROPERTIES
  DEBUG_POSTFIX "_debug"
)

###########################
# Libraries
###########################
target_link_libraries(${PROJECT_NAME}
  sub::lib
)

###########################
# Install
###########################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)