project(app)

###########################
# Targets
###########################
add_executable(${PROJECT_NAME} 
  "src/app.cpp"
)

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
# Libraries
###########################
target_link_libraries(${PROJECT_NAME}
  sub::lib
)

###########################
# Install
###########################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)
