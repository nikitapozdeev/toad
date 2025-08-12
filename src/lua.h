#ifndef TOAD_LUA_H
#define TOAD_LUA_H

#include "vm.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Lua engine structure
typedef struct LuaEngine
{
    lua_State *L;
    VM *vm;
} LuaEngine;

// Lua engine management
LuaEngine *lua_engine_create(VM *vm);
void lua_engine_destroy(LuaEngine *engine);

// Script loading and execution
int lua_load_script(LuaEngine *engine, const char *filename);
int lua_call_update(LuaEngine *engine);
int lua_call_draw(LuaEngine *engine);

// Register all Lua API functions
void lua_api_register(lua_State *L, VM *vm);

// Lua wrapper functions
int lua_putpixel(lua_State *L); // Implementation of putp()
int lua_cos(lua_State *L);      // Implementation of cos()
int lua_sin(lua_State *L);      // Implementation of sin()
int lua_cls(lua_State *L);      // Implementation of cls()
int lua_time(lua_State *L);     // Implementation of time()
int lua_sqrt(lua_State *L);     // Implementation of sqrt()

#endif