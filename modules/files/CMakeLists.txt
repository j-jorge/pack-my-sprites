project( libpms_files )

add_library( pms_files STATIC
  src/pms/files/is_fully_qualified.cpp
  )

target_include_directories(
  pms_files
  PUBLIC
  include
  )

add_subdirectory( tests )
