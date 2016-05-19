if( NOT PMS_INSTALL_EXECUTABLE_DIR )
  set( PMS_INSTALL_EXECUTABLE_DIR bin )
endif()

if( NOT PMS_DEFAULT_SCHEME_PATH )
  set(
    PMS_DEFAULT_SCHEME_PATH
    "${CMAKE_INSTALL_PREFIX}/${PMS_INSTALL_SCRIPT_DIR}"
    )
endif()

include( CMakeLists-auto-pack-my-sprites.txt )
include( CMakeLists-pack-my-sprites.txt )
