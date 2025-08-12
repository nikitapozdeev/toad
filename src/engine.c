#include "engine.h"
#include "lua.h"
#include "js.h"
#include "vm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Lua engine wrapper functions
static int lua_engine_load_script(void *engine_data, const char *filename)
{
    return lua_load_script((LuaEngine *)engine_data, filename);
}

static int lua_engine_call_update(void *engine_data)
{
    return lua_call_update((LuaEngine *)engine_data);
}

static int lua_engine_call_draw(void *engine_data)
{
    return lua_call_draw((LuaEngine *)engine_data);
}

static void lua_engine_destroy_wrapper(void *engine_data)
{
    lua_engine_destroy((LuaEngine *)engine_data);
}

// JavaScript engine wrapper functions
static int js_engine_load_script(void *engine_data, const char *filename)
{
    JSEngine *engine = (JSEngine *)engine_data;
    js_api_register(engine);
    return js_load_script(engine, filename);
}

static int js_engine_call_update(void *engine_data)
{
    return js_call_update((JSEngine *)engine_data);
}

static int js_engine_call_draw(void *engine_data)
{
    return js_call_draw((JSEngine *)engine_data);
}

static void js_engine_destroy_wrapper(void *engine_data)
{
    js_engine_destroy((JSEngine *)engine_data);
}

// Create Lua engine with interface
LanguageEngine *create_lua_engine(VM *vm)
{
    LuaEngine *lua_engine = lua_engine_create(vm);
    if (!lua_engine)
    {
        return NULL;
    }

    LanguageEngine *engine = malloc(sizeof(LanguageEngine));
    if (!engine)
    {
        lua_engine_destroy(lua_engine);
        return NULL;
    }

    engine->engine_data = lua_engine;
    engine->load_script = lua_engine_load_script;
    engine->call_update = lua_engine_call_update;
    engine->call_draw = lua_engine_call_draw;
    engine->destroy = lua_engine_destroy_wrapper;

    return engine;
}

// Create JavaScript engine with interface
LanguageEngine *create_js_engine(VM *vm)
{
    JSEngine *js_engine = js_engine_create(vm);
    if (!js_engine)
    {
        return NULL;
    }

    LanguageEngine *engine = malloc(sizeof(LanguageEngine));
    if (!engine)
    {
        js_engine_destroy(js_engine);
        return NULL;
    }

    engine->engine_data = js_engine;
    engine->load_script = js_engine_load_script;
    engine->call_update = js_engine_call_update;
    engine->call_draw = js_engine_call_draw;
    engine->destroy = js_engine_destroy_wrapper;

    return engine;
}

// Create engine based on file extension
LanguageEngine *create_engine_for_file(VM *vm, const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext)
    {
        fprintf(stderr, "Cannot determine script language from filename: %s\n", filename);
        return NULL;
    }

    if (strcmp(ext, ".lua") == 0)
    {
        return create_lua_engine(vm);
    }
    else if (strcmp(ext, ".js") == 0)
    {
        return create_js_engine(vm);
    }
    else
    {
        fprintf(stderr, "Unsupported script language: %s\n", ext);
        return NULL;
    }
}