include_directories(. bin-packing)

#-------------------------------------------------------------------------------
set( PMS_EXECUTABLE_NAME pack-my-sprites )

#-------------------------------------------------------------------------------
set( PMS_SOURCE_FILES
  code/application.cpp
  code/main.cpp
  )

#-------------------------------------------------------------------------------
# Boost
include(FindBoost)

find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem system
  )

#-------------------------------------------------------------------------------
# Claw
find_package(libclaw 1.7.4)

#-------------------------------------------------------------------------------
add_executable(
  ${PMS_EXECUTABLE_NAME}
  ${PMS_SOURCE_FILES}
  )

if( PMS_DEFAULT_SCHEME_PATH )
  set(
    PMS_COMPILE_DEFINITIONS
    ${PMS_COMPILE_DEFINITIONS}
    "PMS_DEFAULT_SCHEME_PATH=${PMS_DEFAULT_SCHEME_PATH}"
    )
endif()

set_target_properties(
  ${PMS_EXECUTABLE_NAME}
  PROPERTIES
  COMPILE_DEFINITIONS
  "${PMS_COMPILE_DEFINITIONS}"
  CXX_STANDARD
  11
  )

target_link_libraries(
  ${PMS_EXECUTABLE_NAME}
  pms_generators
  pms_serialization
  ${CLAW_APPLICATION_LIBRARIES}
  )

install(
  TARGETS ${PMS_EXECUTABLE_NAME}
  DESTINATION ${PMS_INSTALL_EXECUTABLE_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
  )
