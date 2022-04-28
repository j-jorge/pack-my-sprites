cmake_minimum_required( VERSION 3.14 )

project(pack-my-sprites)

enable_testing()

function( declare_test SOURCE NAME )
  add_executable( ${NAME}-test ${SOURCE} )
  add_test( ${NAME} ${NAME}-test )

  foreach( LIB ${ARGN} )
    target_link_libraries( ${NAME}-test ${LIB} )
  endforeach()

  target_link_libraries( ${NAME}-test utest )
endfunction()

if( NOT PMS_INSTALL_RESOURCES_DIR )
  set( PMS_INSTALL_RESOURCES_DIR share/pack-my-sprites )
endif()

set( PMS_INSTALL_SCRIPT_DIR "${PMS_INSTALL_RESOURCES_DIR}/script" )

if( NOT PMS_DEFAULT_SCHEME_PATH )
  set(
    PMS_DEFAULT_SCHEME_PATH
    "${CMAKE_INSTALL_PREFIX}/${PMS_INSTALL_SCRIPT_DIR}"
    )
endif()

function( install_script SOURCE )

  install(
    FILES ${SOURCE}
    DESTINATION ${PMS_INSTALL_SCRIPT_DIR}

    PERMISSIONS OWNER_READ OWNER_WRITE
    GROUP_READ
    WORLD_READ
    )
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
