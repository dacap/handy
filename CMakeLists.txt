# handy text editor
# Copyright (c) 2016 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

cmake_minimum_required(VERSION 3.0)

find_library(CURSES_LIB NAMES curses)

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

add_executable(handy
  app.cpp
  clipboard.cpp
  doc_view.cpp
  fs.cpp
  main.cpp
  term.cpp)

add_executable(getkey
  tools/getkey.cpp)

target_link_libraries(handy ${CURSES_LIB})
target_link_libraries(getkey ${CURSES_LIB})
