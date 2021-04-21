# handy text editor
# Copyright (c) 2021 David Capello
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

if(UNIX)
  add_executable(getkey getkey.cpp)
  target_link_libraries(getkey term)
endif()
