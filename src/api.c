#include "api.h"
#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Palette (16 colors, RGB format)
const u32 PALETTE[PALETTE_SIZE] = {
    0x000000, // 0  - Black
    0x1D2B53, // 1  - Dark Blue
    0x7E2553, // 2  - Dark Purple
    0x008751, // 3  - Dark Green
    0xAB5236, // 4  - Brown
    0x5F574F, // 5  - Dark Gray
    0xC2C3C7, // 6  - Light Gray
    0xFFF1E8, // 7  - White
    0xFF004D, // 8  - Red
    0xFFA300, // 9  - Orange
    0xFFEC27, // 10 - Yellow
    0x00E436, // 11 - Green
    0x29ADFF, // 12 - Blue
    0x83769C, // 13 - Indigo
    0xFF77A8, // 14 - Pink
    0xFFCCAA  // 15 - Peach
};

int api_putpixel(lua_State *L)
{
    int x = (int)luaL_checknumber(L, 1);
    int y = (int)luaL_checknumber(L, 2);
    int color = luaL_checkinteger(L, 3);

    // Get VM from Lua registry
    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H && color >= 0 && color < PALETTE_SIZE)
    {
        vm->pixels[y * SCREEN_W + x] = color;
    }

    return 0;
}

int api_cos(lua_State *L)
{
    double angle = luaL_checknumber(L, 1);
    // In PICO-8 full rotation = 1, not 2π
    // Convert from PICO-8 format to radians
    double radians = angle * 2.0 * M_PI;
    double result = cos(radians);
    lua_pushnumber(L, result);
    return 1;
}

int api_cls(lua_State *L)
{
    // Get VM from Lua registry
    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    // Clear graphics buffer
    memset(vm->pixels, 0, SCREEN_W * SCREEN_H);
    return 0;
}

int api_time(lua_State *L)
{
    // Get VM from Lua registry
    lua_getglobal(L, "__vm");
    VM *vm = (VM *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    // Return time in seconds since startup
    u32 current_time = SDL_GetTicks();
    double elapsed_time = (current_time - vm->start_time) / 1000.0;
    lua_pushnumber(L, elapsed_time);
    return 1;
}

int api_sin(lua_State *L)
{
    double angle = luaL_checknumber(L, 1);
    // In PICO-8 sin(x) = cos(x - 0.25) (shift by -90 degrees)
    // Convert from PICO-8 format to radians
    double radians = (angle - 0.25) * 2.0 * M_PI;
    double result = cos(radians);
    lua_pushnumber(L, result);
    return 1;
}

int api_sqrt(lua_State *L)
{
    double number = luaL_checknumber(L, 1);
    double result = sqrt(number);
    lua_pushnumber(L, result);
    return 1;
}

void api_register(lua_State *L, VM *vm)
{
    // Register functions
    lua_register(L, "putp", api_putpixel);
    lua_register(L, "cos", api_cos);
    lua_register(L, "sin", api_sin);
    lua_register(L, "cls", api_cls);
    lua_register(L, "time", api_time);
    lua_register(L, "sqrt", api_sqrt);

    // Can add constants for colors
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