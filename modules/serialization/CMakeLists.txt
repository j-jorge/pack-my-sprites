project( libpms_serialization )

add_library( pms_serialization STATIC
  src/pms/serialization/read_spritedesc.cpp

  src/pms/serialization/spritedesc/node_parser.cpp
  src/pms/serialization/spritedesc/node_parser_atlas.cpp
  src/pms/serialization/spritedesc/node_parser_file.cpp
  src/pms/serialization/spritedesc/node_parser_image_entry.cpp
  src/pms/serialization/spritedesc/node_parser_image_size.cpp
  src/pms/serialization/spritedesc/node_parser_sprite_declaration.cpp
  src/pms/serialization/spritedesc/node_parser_sprite_description_base.cpp
  src/pms/serialization/spritedesc/parser.cpp
  )

target_include_directories(
  pms_serialization
  PUBLIC include
  )

target_link_libraries(
  pms_serialization
  PUBLIC
  pms_layout
)
