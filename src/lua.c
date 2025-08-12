#include "lua.h"
#include "api.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

// Lua wrapper for putpixel api call
int lua_putpixel(lua_State *L)
{
    int x = (int)luaL_checknumber(L, 1);
    int y = (int)luaL_checknumber(L, 2);
    int color = luaL_checkinteger(L, 3);

    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    api_putpixel(vm, x, y, color);
    return 0;
}

// Lua wrapper for cos api call
int lua_cos(lua_State *L)
{
    double angle = luaL_checknumber(L, 1);
    double result = api_cos(angle);
    lua_pushnumber(L, result);
    return 1;
}

// Lua wrapper for sin api call
int lua_sin(lua_State *L)
{
    double angle = luaL_checknumber(L, 1);
    double result = api_sin(angle);
    lua_pushnumber(L, result);
    return 1;
}

// Lua wrapper for cls api call
int lua_cls(lua_State *L)
{
    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    api_cls(vm);
    return 0;
}

// Lua wrapper for time api call
int lua_time(lua_State *L)
{
    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    double elapsed_time = api_time(vm);
    lua_pushnumber(L, elapsed_time);
    return 1;
}

// Lua wrapper for sqrt api call
int lua_sqrt(lua_State *L)
{
    double number = luaL_checknumber(L, 1);
    double result = api_sqrt(number);
    lua_pushnumber(L, result);
    return 1;
}

// Create Lua engine
LuaEngine *lua_engine_create(VM *vm)
{
    LuaEngine *engine = malloc(sizeof(LuaEngine));
    if (!engine)
    {
        return NULL;
    }

    engine->vm = vm;
    engine->L = luaL_newstate();
    if (!engine->L)
    {
        free(engine);
        return NULL;
    }

    luaL_openlibs(engine->L);
    lua_api_register(engine->L, vm);

    return engine;
}

// Destroy Lua engine
void lua_engine_destroy(LuaEngine *engine)
{
    if (engine)
    {
        if (engine->L)
        {
            lua_close(engine->L);
        }
        free(engine);
    }
}

// Load and execute Lua script
int lua_load_script(LuaEngine *engine, const char *filename)
{
    if (!engine || !engine->L)
    {
        return -1;
    }

    if (luaL_dofile(engine->L, filename) != LUA_OK)
    {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(engine->L, -1));
        return -1;
    }

    return 0;
}

// Lua wrapper for update api call
int lua_call_update(LuaEngine *engine)
{
    if (!engine || !engine->L)
    {
        return -1;
    }

    lua_getglobal(engine->L, "update");
    if (lua_isfunction(engine->L, -1))
    {
        if (lua_pcall(engine->L, 0, 0, 0) != LUA_OK)
        {
            fprintf(stderr, "Error in update(): %s\n", lua_tostring(engine->L, -1));
            return -1;
        }
    }
    else
    {
        lua_pop(engine->L, 1);
    }

    return 0;
}

// Lua wrapper for draw api call
int lua_call_draw(LuaEngine *engine)
{
    if (!engine || !engine->L)
    {
        return -1;
    }

    lua_getglobal(engine->L, "draw");
    if (lua_isfunction(engine->L, -1))
    {
        if (lua_pcall(engine->L, 0, 0, 0) != LUA_OK)
        {
            fprintf(stderr, "Error in draw(): %s\n", lua_tostring(engine->L, -1));
            return -1;
        }
    }
    else
    {
        lua_pop(engine->L, 1);
    }

    return 0;
}

void lua_api_register(lua_State *L, VM *vm)
{
    // Register Lua wrapper functions
    lua_register(L, "putp", lua_putpixel);
    lua_register(L, "cos", lua_cos);
    lua_register(L, "sin", lua_sin);
    lua_register(L, "cls", lua_cls);
    lua_register(L, "time", lua_time);
    lua_register(L, "sqrt", lua_sqrt);

    // Add color constants
    for (int i = 0; i < PALETTE_SIZE; i++)
    {
        char color_name[32];
        snprintf(color_name, sizeof(color_name), "COLOR_%d", i);
        lua_pushinteger(L, i);
        lua_setglobal(L, color_name);
    }

    // Add VM to Lua global scope
    lua_pushlightuserdata(L, vm);
    lua_setglobal(L, "__vm");
}