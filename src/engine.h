#ifndef TOAD_ENGINE_H
#define TOAD_ENGINE_H

#include "vm.h"

// Language engine interface
typedef struct LanguageEngine
{
    void *engine_data; // Pointer to actual engine (LuaEngine* or JSEngine*)

    // Function pointers for engine operations
    int (*load_script)(void *engine_data, const char *filename);
    int (*call_update)(void *engine_data);
    int (*call_draw)(void *engine_data);
    void (*destroy)(void *engine_data);
} LanguageEngine;

// Engine creation functions
LanguageEngine *create_lua_engine(VM *vm);
LanguageEngine *create_js_engine(VM *vm);

// Determine engine type by file extension
LanguageEngine *create_engine_for_file(VM *vm, const char *filename);

#endif