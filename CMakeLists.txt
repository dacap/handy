# handy text editor
# Copyright (c) 2016-2021 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(handy)

add_subdirectory(fmt)

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

set(CLIP_ENABLE_IMAGE OFF CACHE BOOL "")
set(CLIP_EXAMPLES OFF CACHE BOOL "")
set(LAF_WITH_EXAMPLES OFF CACHE BOOL "")
set(LAF_WITH_TESTS OFF CACHE BOOL "")
add_subdirectory(laf)

set(UNDO_TESTS OFF CACHE BOOL "Compile undo tests")
add_subdirectory(undo)
target_include_directories(undo PUBLIC .)

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_library(lua
  lua/lapi.c
  lua/lcode.c
  lua/lctype.c
  lua/ldebug.c
  lua/ldo.c
  lua/ldump.c
  lua/lfunc.c
  lua/lgc.c
  lua/llex.c
  lua/lmem.c
  lua/lobject.c
  lua/lopcodes.c
  lua/lparser.c
  lua/lstate.c
  lua/lstring.c
  lua/ltable.c
  lua/ltm.c
  lua/lundump.c
  lua/lvm.c
  lua/lzio.c
  lua/ltests.c)
add_library(lauxlib lua/lauxlib.c)
add_library(lualib
  lua/lbaselib.c
  lua/lcorolib.c
  lua/ldblib.c
  lua/linit.c
  lua/liolib.c
  lua/lmathlib.c
  lua/loadlib.c
  lua/loslib.c
  lua/lstrlib.c
  lua/ltablib.c
  lua/lutf8lib.c)
target_include_directories(lua PUBLIC lua)
target_include_directories(lauxlib PUBLIC lua)
target_include_directories(lualib PUBLIC lua)
target_link_libraries(lauxlib lua)

add_subdirectory(term)
add_subdirectory(app)
add_subdirectory(tools)
