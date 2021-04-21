# handy text editor
# Copyright (c) 2021 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

add_executable(handy
  alert_view.cpp
  app.cpp
  clipboard.cpp
  doc.cpp
  doc_undo.cpp
  doc_view.cpp
  main.cpp
  open_file_view.cpp
  view.cpp)

target_link_libraries(handy
  term
  clip
  undo
  laf-base
  ${PLATFORM_LIBS})

set_target_properties(handy
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
