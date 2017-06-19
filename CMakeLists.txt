# handy text editor
# Copyright (c) 2016-2017 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

cmake_minimum_required(VERSION 3.0)

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

set(LAF_WITH_TESTS OFF CACHE BOOL "Enable LAF tests")
add_subdirectory(laf)

set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
add_subdirectory(clip)

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

set(PLATFORM_LIBS)
set(TERM_SOURCES)
if(UNIX)
  find_library(CURSES_LIB NAMES curses)
  list(APPEND PLATFORM_LIBS ${CURSES_LIB})
  list(APPEND TERM_SOURCES term_curses.cpp)
elseif(WIN32)
  list(APPEND TERM_SOURCES term_win.cpp)
endif()

add_library(term-lib ${TERM_SOURCES})

add_executable(handy
  app.cpp
  clipboard.cpp
  doc_view.cpp
  main.cpp
  view.cpp)
target_link_libraries(handy
  term-lib
  clip
  laf-base
  ${PLATFORM_LIBS})

if(UNIX)
  add_executable(getkey tools/getkey.cpp)
  target_link_libraries(getkey ${PLATFORM_LIBS})
endif()
