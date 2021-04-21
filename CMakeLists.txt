# handy text editor
# Copyright (c) 2016-2021 David Capello
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

set(UNDO_TESTS OFF CACHE BOOL "Compile undo tests")
add_subdirectory(undo)
target_include_directories(undo PUBLIC .)

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

add_subdirectory(term)
add_subdirectory(app)
add_subdirectory(tools)
