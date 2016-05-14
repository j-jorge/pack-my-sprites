project( libpms_packing )

add_library( pms_packing STATIC
  src/rbp/MaxRectsBinPack.cpp
  src/rbp/Rect.cpp
  )

target_include_directories(
  pms_packing
  PUBLIC include
  )
