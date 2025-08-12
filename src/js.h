#ifndef TOAD_JS_H
#define TOAD_JS_H

#include "vm.h"
#include "../vendor/quickjs/quickjs.h"

// JavaScript context structure
typedef struct
{
    JSRuntime *rt;
    JSContext *ctx;
    VM *vm;
} JSEngine;

// Initialize JavaScript engine
JSEngine *js_engine_create(VM *vm);
void js_engine_destroy(JSEngine *engine);

// Register all JavaScript API functions
void js_api_register(JSEngine *engine);

// Load and execute JavaScript file
int js_load_script(JSEngine *engine, const char *filename);

// Call JavaScript functions
int js_call_update(JSEngine *engine);
int js_call_draw(JSEngine *engine);

// JavaScript wrapper functions
JSValue js_putpixel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_cos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_sin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_cls(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_sqrt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif