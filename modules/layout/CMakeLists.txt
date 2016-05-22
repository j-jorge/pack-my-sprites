project( libpms_layout )

add_library( pms_layout STATIC
  src/pms/layout/atlas.cpp
  src/pms/layout/build.cpp
  src/pms/layout/description.cpp
  )

target_include_directories(
  pms_layout
  PUBLIC include
  )

target_link_libraries(
  pms_layout
  PUBLIC
  pms_resources
  pms_packing
)

add_subdirectory( tests )
