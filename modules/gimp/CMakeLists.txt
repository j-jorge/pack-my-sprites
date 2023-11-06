project( libpms_gimp )

add_library( pms_gimp STATIC
  src/pms/gimp/system_interface.cpp
  )

target_include_directories(
  pms_gimp
  PUBLIC
  include
  )

target_link_libraries( pms_gimp PUBLIC ${Boost_LIBRARIES} )
