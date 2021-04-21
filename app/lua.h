// handy text editor
// Copyright (c) 2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

#include <string>

class Lua {
public:
  Lua();
  ~Lua();

  void run_script(const std::string& fn);
  void run_code(const std::string& code);

private:
  lua_State* L;
};
