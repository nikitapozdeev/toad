#include "js.h"
#include "api.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// JavaScript wrapper for putpixel api call
JSValue js_putpixel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc != 3)
    {
        return JS_ThrowTypeError(ctx, "putp() requires 3 arguments: x, y, color");
    }

    int x, y, color;
    if (JS_ToInt32(ctx, &x, argv[0]) < 0 ||
        JS_ToInt32(ctx, &y, argv[1]) < 0 ||
        JS_ToInt32(ctx, &color, argv[2]) < 0)
    {
        return JS_EXCEPTION;
    }

    JSEngine *engine = (JSEngine *)JS_GetContextOpaque(ctx);
    if (!engine || !engine->vm)
    {
        return JS_ThrowInternalError(ctx, "VM not available");
    }

    api_putpixel(engine->vm, x, y, color);
    return JS_UNDEFINED;
}

// JavaScript wrapper for cos api call
JSValue js_cos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc != 1)
    {
        return JS_ThrowTypeError(ctx, "cos() requires 1 argument: angle");
    }

    double angle;
    if (JS_ToFloat64(ctx, &angle, argv[0]) < 0)
    {
        return JS_EXCEPTION;
    }

    double result = api_cos(angle);
    return JS_NewFloat64(ctx, result);
}

// JavaScript wrapper for sin api call
JSValue js_sin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc != 1)
    {
        return JS_ThrowTypeError(ctx, "sin() requires 1 argument: angle");
    }

    double angle;
    if (JS_ToFloat64(ctx, &angle, argv[0]) < 0)
    {
        return JS_EXCEPTION;
    }

    double result = api_sin(angle);
    return JS_NewFloat64(ctx, result);
}

// JavaScript wrapper for cls api call
JSValue js_cls(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    JSEngine *engine = (JSEngine *)JS_GetContextOpaque(ctx);
    if (!engine || !engine->vm)
    {
        return JS_ThrowInternalError(ctx, "VM not available");
    }

    api_cls(engine->vm);
    return JS_UNDEFINED;
}

// JavaScript wrapper for time api call
JSValue js_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    JSEngine *engine = (JSEngine *)JS_GetContextOpaque(ctx);
    if (!engine || !engine->vm)
    {
        return JS_ThrowInternalError(ctx, "VM not available");
    }

    double elapsed_time = api_time(engine->vm);
    return JS_NewFloat64(ctx, elapsed_time);
}

// JavaScript wrapper for sqrt api call
JSValue js_sqrt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc != 1)
    {
        return JS_ThrowTypeError(ctx, "sqrt() requires 1 argument: number");
    }

    double number;
    if (JS_ToFloat64(ctx, &number, argv[0]) < 0)
    {
        return JS_EXCEPTION;
    }

    double result = api_sqrt(number);
    return JS_NewFloat64(ctx, result);
}

// Create JavaScript engine
JSEngine *js_engine_create(VM *vm)
{
    JSEngine *engine = malloc(sizeof(JSEngine));
    if (!engine)
    {
        return NULL;
    }

    engine->vm = vm;
    engine->rt = JS_NewRuntime();
    if (!engine->rt)
    {
        free(engine);
        return NULL;
    }

    engine->ctx = JS_NewContext(engine->rt);
    if (!engine->ctx)
    {
        JS_FreeRuntime(engine->rt);
        free(engine);
        return NULL;
    }

    // Set engine as context opaque data for access in wrapper functions
    JS_SetContextOpaque(engine->ctx, engine);

    return engine;
}

// Destroy JavaScript engine
void js_engine_destroy(JSEngine *engine)
{
    if (engine)
    {
        if (engine->ctx)
        {
            JS_FreeContext(engine->ctx);
        }
        if (engine->rt)
        {
            JS_FreeRuntime(engine->rt);
        }
        free(engine);
    }
}

// Register JavaScript API functions
void js_api_register(JSEngine *engine)
{
    if (!engine || !engine->ctx)
    {
        return;
    }

    JSContext *ctx = engine->ctx;
    JSValue global = JS_GetGlobalObject(ctx);

    // Register API functions
    JS_SetPropertyStr(ctx, global, "putp", JS_NewCFunction(ctx, js_putpixel, "putp", 3));
    JS_SetPropertyStr(ctx, global, "cos", JS_NewCFunction(ctx, js_cos, "cos", 1));
    JS_SetPropertyStr(ctx, global, "sin", JS_NewCFunction(ctx, js_sin, "sin", 1));
    JS_SetPropertyStr(ctx, global, "cls", JS_NewCFunction(ctx, js_cls, "cls", 0));
    JS_SetPropertyStr(ctx, global, "time", JS_NewCFunction(ctx, js_time, "time", 0));
    JS_SetPropertyStr(ctx, global, "sqrt", JS_NewCFunction(ctx, js_sqrt, "sqrt", 1));

    // Add color constants
    for (int i = 0; i < PALETTE_SIZE; i++)
    {
        char color_name[32];
        snprintf(color_name, sizeof(color_name), "COLOR_%d", i);
        JS_SetPropertyStr(ctx, global, color_name, JS_NewInt32(ctx, i));
    }

    JS_FreeValue(ctx, global);
}

// Load and execute JavaScript file
int js_load_script(JSEngine *engine, const char *filename)
{
    if (!engine || !engine->ctx)
    {
        return -1;
    }

    // Read file content
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Cannot open JavaScript file: %s\n", filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(file_size + 1);
    if (!content)
    {
        fclose(file);
        return -1;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);

    // Execute JavaScript code
    JSValue result = JS_Eval(engine->ctx, content, file_size, filename, JS_EVAL_TYPE_GLOBAL);
    free(content);

    if (JS_IsException(result))
    {
        JSValue exception = JS_GetException(engine->ctx);
        const char *error_str = JS_ToCString(engine->ctx, exception);
        fprintf(stderr, "JavaScript error: %s\n", error_str);
        JS_FreeCString(engine->ctx, error_str);
        JS_FreeValue(engine->ctx, exception);
        JS_FreeValue(engine->ctx, result);
        return -1;
    }

    JS_FreeValue(engine->ctx, result);
    return 0;
}

// JavaScript wrapper for update api call
int js_call_update(JSEngine *engine)
{
    if (!engine || !engine->ctx)
    {
        return -1;
    }

    JSValue global = JS_GetGlobalObject(engine->ctx);
    JSValue update_func = JS_GetPropertyStr(engine->ctx, global, "update");

    if (JS_IsFunction(engine->ctx, update_func))
    {
        JSValue result = JS_Call(engine->ctx, update_func, global, 0, NULL);
        if (JS_IsException(result))
        {
            JSValue exception = JS_GetException(engine->ctx);
            const char *error_str = JS_ToCString(engine->ctx, exception);
            fprintf(stderr, "Error in update(): %s\n", error_str);
            JS_FreeCString(engine->ctx, error_str);
            JS_FreeValue(engine->ctx, exception);
            JS_FreeValue(engine->ctx, result);
            JS_FreeValue(engine->ctx, update_func);
            JS_FreeValue(engine->ctx, global);
            return -1;
        }
        JS_FreeValue(engine->ctx, result);
    }

    JS_FreeValue(engine->ctx, update_func);
    JS_FreeValue(engine->ctx, global);
    return 0;
}

// JavaScript wrapper for draw api call
int js_call_draw(JSEngine *engine)
{
    if (!engine || !engine->ctx)
    {
        return -1;
    }

    JSValue global = JS_GetGlobalObject(engine->ctx);
    JSValue draw_func = JS_GetPropertyStr(engine->ctx, global, "draw");

    if (JS_IsFunction(engine->ctx, draw_func))
    {
        JSValue result = JS_Call(engine->ctx, draw_func, global, 0, NULL);
        if (JS_IsException(result))
        {
            JSValue exception = JS_GetException(engine->ctx);
            const char *error_str = JS_ToCString(engine->ctx, exception);
            fprintf(stderr, "Error in draw(): %s\n", error_str);
            JS_FreeCString(engine->ctx, error_str);
            JS_FreeValue(engine->ctx, exception);
            JS_FreeValue(engine->ctx, result);
            JS_FreeValue(engine->ctx, draw_func);
            JS_FreeValue(engine->ctx, global);
            return -1;
        }
        JS_FreeValue(engine->ctx, result);
    }

    JS_FreeValue(engine->ctx, draw_func);
    JS_FreeValue(engine->ctx, global);
    return 0;
}