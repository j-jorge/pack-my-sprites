include_directories(. bin-packing)

#-------------------------------------------------------------------------------
set( PMS_EXECUTABLE_NAME pack-my-sprites )

#-------------------------------------------------------------------------------
set( PMS_SOURCE_FILES
  bin-packing/MaxRectsBinPack.cpp
  bin-packing/Rect.cpp

  code/application.cpp
  code/css_generator.cpp
  code/gimp_interface.cpp
  code/image_generator.cpp
  code/main.cpp
  code/parser.cpp
  code/plist_generator.cpp
  code/sprite_sheet.cpp
  code/sprite_sheet_builder.cpp
  code/spritedesc.cpp
  code/spritepos_generator.cpp
  code/xcf_info.cpp
  code/xcf_map.cpp
  code/working_directory.cpp

  node_parser/code/node_parser.cpp
  node_parser/code/node_parser_file.cpp
  node_parser/code/node_parser_sprite_declaration.cpp
  node_parser/code/node_parser_sprite_description_base.cpp
  node_parser/code/node_parser_sprite_sheet.cpp
  node_parser/code/node_parser_xcf_entry.cpp
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
  )

target_link_libraries(
  ${PMS_EXECUTABLE_NAME}
  ${CLAW_APPLICATION_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

install(
  TARGETS ${PMS_EXECUTABLE_NAME}
  DESTINATION ${PMS_INSTALL_EXECUTABLE_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
  )
