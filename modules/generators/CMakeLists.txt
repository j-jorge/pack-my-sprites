project( libpms_generators )

install_script( scripts/common.scm )

add_library( pms_generators STATIC
  src/pms/generators/css.cpp
  src/pms/generators/plist.cpp
  src/pms/generators/png.cpp
  src/pms/generators/spritepos.cpp

  src/pms/generators/detail/working_directory.cpp
  )

target_include_directories(
  pms_generators
  PUBLIC include
  )

target_link_libraries(
  pms_generators
  PUBLIC
  pms_layout
)
