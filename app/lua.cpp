// handy text editor
// Copyright (c) 2021-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "lua.h"

#include "base/fs.h"
#include "base/fstream_path.h"
#include "ctx.h"

#include <fstream>
#include <sstream>

Lua::Lua()
  : L(luaL_newstate())
{
  luaL_requiref(L, LUA_GNAME, luaopen_base, 1);
  luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
  luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
  luaL_requiref(L, LUA_IOLIBNAME, luaopen_io, 1);
  luaL_requiref(L, LUA_OSLIBNAME, luaopen_os, 1);
  luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
  luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
  luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
  luaL_requiref(L, LUA_DBLIBNAME, luaopen_debug, 1);
  lua_pop(L, 9);
}

Lua::~Lua()
{
  lua_close(L);
}

Lua::Error Lua::run_script(Ctx* ctx, const std::string& fn)
{
  std::stringstream buf;
  {
    std::ifstream s(FSTREAM_PATH(fn));
    if (!s)
      return Error::FileNotFound;
    buf << s.rdbuf();
  }
  std::string abs_fn = base::get_absolute_path(fn);
  abs_fn.insert(0, "@");

  Error err = Error::OK;
  if (luaL_loadbuffer(L, buf.str().c_str(), buf.str().size(), abs_fn.c_str()) ||
      lua_pcall(L, 0, 1, 0)) {
    if (const char* s = lua_tostring(L, -1))
      ctx->alert(s);
    err = Error::LuaError;
  }
  lua_gc(L, LUA_GCCOLLECT);

  return err;
}

Lua::Error Lua::run_code(Ctx* ctx, const std::string& code)
{
  Error err = Error::OK;

  if (luaL_loadbuffer(L, code.c_str(), code.size(), "") ||
      lua_pcall(L, 0, 1, 0)) {
    if (const char* s = lua_tostring(L, -1))
      ctx->alert(s);
    err = Error::LuaError;
  }
  lua_gc(L, LUA_GCCOLLECT);

  return err;
}
