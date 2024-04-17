// handy text editor
// Copyright (c) 2021-2024 David Capello
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

class Ctx;

class Lua {
public:
  enum Error {
    OK,
    FileNotFound,
    LuaError,
  };

  Lua();
  ~Lua();

  Error run_script(Ctx* ctx, const std::string& fn);
  Error run_code(Ctx* ctx, const std::string& code);

private:
  lua_State* L;
};
