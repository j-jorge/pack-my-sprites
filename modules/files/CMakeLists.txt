project( libpms_files )

find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem system
  )

add_library( pms_files STATIC
  src/pms/files/is_fully_qualified.cpp
  )

target_include_directories(
  pms_files
  PUBLIC
  include
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries( pms_files PUBLIC ${Boost_LIBRARIES} )

add_subdirectory( tests )
