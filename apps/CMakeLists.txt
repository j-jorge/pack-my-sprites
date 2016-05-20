if( NOT PMS_INSTALL_EXECUTABLE_DIR )
  set( PMS_INSTALL_EXECUTABLE_DIR bin )
endif()

include( CMakeLists-auto-pack-my-sprites.txt )
include( CMakeLists-pack-my-sprites.txt )
