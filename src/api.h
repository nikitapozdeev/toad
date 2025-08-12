#ifndef TOAD_API_H
#define TOAD_API_H

#include "vm.h"
#include "defs.h"

// Register all API functions in Lua
void api_register(lua_State *L, VM *vm);

// Color palette (accessible from Lua)
extern const u32 PALETTE[PALETTE_SIZE];

// Individual API functions
int api_putpixel(lua_State *L); // Implementation of putp()
int api_cos(lua_State *L);      // Implementation of cos()
int api_sin(lua_State *L);      // Implementation of sin()
int api_cls(lua_State *L);      // Implementation of cls()
int api_time(lua_State *L);     // Implementation of time()
int api_sqrt(lua_State *L);     // Implementation of sqrt()

#endif