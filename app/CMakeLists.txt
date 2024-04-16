# handy text editor
# Copyright (c) 2021-2024 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/handy.lua
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/handy.lua ${CMAKE_BINARY_DIR}/handy.lua
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/handy.lua)

add_executable(handy
  alert_view.cpp
  app.cpp
  clipboard.cpp
  doc.cpp
  doc_undo.cpp
  doc_view.cpp
  git_view.cpp
  lua.cpp
  main.cpp
  open_file_view.cpp
  view.cpp
  ${CMAKE_BINARY_DIR}/handy.lua)

target_link_libraries(handy
  fmt
  term
  clip
  undo
  laf-base
  lua lauxlib lualib
  ${PLATFORM_LIBS})

set_target_properties(handy
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
