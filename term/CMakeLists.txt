# handy text editor
# Copyright (c) 2021-2024 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

set(PLATFORM_LIBS)
set(TERM_SOURCES)
if(UNIX)
  find_library(CURSES_LIB NAMES curses)
  list(APPEND PLATFORM_LIBS ${CURSES_LIB})
  list(APPEND TERM_SOURCES term_curses.cpp)
elseif(WIN32)
  list(APPEND TERM_SOURCES term_win.cpp)
endif()

if(LAF_BACKEND STREQUAL "skia")
  list(APPEND TERM_SOURCES term_laf.cpp)
endif()

add_library(term ${TERM_SOURCES})
target_include_directories(term PUBLIC .)
target_link_libraries(term ${PLATFORM_LIBS})
target_link_libraries(term laf-base)

if(LAF_BACKEND STREQUAL "skia")
  target_link_libraries(term laf-os laf-text)
endif()
