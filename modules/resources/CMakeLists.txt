project( libpms_resources )

install_script( scripts/info.scm )

find_package( Boost 1.42 REQUIRED )
find_package( claw 1.8.0 REQUIRED )

add_library( pms_resources STATIC
  src/pms/resources/image.cpp
  src/pms/resources/image_mapping.cpp
  )

target_include_directories(
  pms_resources
  PUBLIC include
  )

target_link_libraries(
  pms_resources
  PUBLIC
  claw_graphic
  claw_logger
  pms_files
  pms_gimp
)

add_subdirectory( tests )
