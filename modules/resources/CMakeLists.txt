project( libpms_resources )

find_package( Boost 1.42 REQUIRED )
find_package( libclaw 1.7.4 )

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
  ${CLAW_GRAPHIC_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  pms_files
  pms_gimp
)

add_subdirectory( tests )
