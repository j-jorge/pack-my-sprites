cmake_minimum_required( VERSION 2.8 )

project( pack-my-sprites )

#-------------------------------------------------------------------------------
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin" )

#-------------------------------------------------------------------------------
# where the programs are installed
if( NOT SDC_INSTALL_EXECUTABLE_DIR )
  set( SDC_INSTALL_EXECUTABLE_DIR bin )
endif( NOT SDC_INSTALL_EXECUTABLE_DIR )

# where the resources are installed
if( NOT SDC_INSTALL_RESOURCES_DIR )
  set( SDC_INSTALL_RESOURCES_DIR share/pack-my-sprites )
endif( NOT SDC_INSTALL_RESOURCES_DIR )

#-------------------------------------------------------------------------------
subdirs(
  src
  desktop
  )

#-------------------------------------------------------------------------------
# target to uninstall files
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(
  uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  )
