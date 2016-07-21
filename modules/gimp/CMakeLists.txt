project( libpms_gimp )

find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem system
  )

add_library( pms_gimp STATIC
  src/pms/gimp/system_interface.cpp
  )

target_include_directories(
  pms_gimp
  PUBLIC
  include
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries( pms_gimp PUBLIC ${Boost_LIBRARIES} )
