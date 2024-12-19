project( libpms_app )

find_package( Boost 1.42 REQUIRED COMPONENTS program_options )

if( PMS_DEFAULT_SCHEME_PATH )
  set(
    PMS_COMPILE_DEFINITIONS
    ${PMS_COMPILE_DEFINITIONS}
    "PMS_DEFAULT_SCHEME_PATH=${PMS_DEFAULT_SCHEME_PATH}"
    )
endif()

add_library( pms_app STATIC
  src/pms/app/general_program_options.cpp
  src/pms/app/packer.cpp
  src/pms/app/packer_program_options.cpp
  src/pms/app/packer_options.cpp
  )

set_target_properties(
  pms_app
  PROPERTIES
  COMPILE_DEFINITIONS
  "${PMS_COMPILE_DEFINITIONS}"
  )

target_include_directories(
  pms_app
  PUBLIC include
  )

target_link_libraries(
  pms_app
  PUBLIC
  pms_generators
  pms_serialization
  ${Boost_LIBRARIES}
)
