option(PMS_BUILD_MAN_PAGES "Whether to build the man pages or not." TRUE)

if (NOT PMS_BUILD_MAN_PAGES)
  return()
endif()

find_program(DOCBOOK_TO_MAN docbook-to-man ENV)

#-------------------------------------------------------------------------------
# Use this macro to create man pages with docbook-to-man
macro(DOCBOOK_CREATE_MAN _targetName _volume _first_sgml_file)
  foreach (_file ${_first_sgml_file} ${ARGN})
    get_filename_component(_absFile ${_file} ABSOLUTE)
    get_filename_component(_abs_PATH ${_absFile} PATH)
    get_filename_component(_prog ${_absFile} NAME_WE)
    set(_man_file ${CMAKE_CURRENT_BINARY_DIR}/${_prog}.${_volume})

    add_custom_command(
      OUTPUT ${_man_file}
      COMMAND ${DOCBOOK_TO_MAN} ${_absFile} > ${_man_file}
      DEPENDS ${_absFile}
      )

    install(
      FILES ${_man_file}
      DESTINATION share/man/man${_volume}
      )

    set(_man_files ${_man_files} ${_man_file})
  endforeach()

  add_custom_target(${_targetName} ALL DEPENDS ${_man_files})
endmacro()

#-------------------------------------------------------------------------------
docbook_create_man(
  pms-man 1
  pack-my-sprites.sgml
  auto-pack-my-sprites.sgml
  )
