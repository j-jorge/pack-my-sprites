cmake_minimum_required( VERSION 2.8 )

enable_testing()

function( declare_test SOURCE NAME )
  add_executable( ${NAME}-test ${SOURCE} )
  add_test( ${NAME} ${NAME}-test )

  foreach( LIB ${ARGN} )
    target_link_libraries( ${NAME}-test ${LIB} )
  endforeach()
    
  target_link_libraries( ${NAME}-test utest )
endfunction()

add_subdirectory( modules )
add_subdirectory( apps )
add_subdirectory( docs )

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(
  uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  )
