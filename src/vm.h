#ifndef TOAD_VM_H
#define TOAD_VM_H

#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "types.h"

typedef struct
{
    lua_State *L;           // Lua state
    SDL_Renderer *renderer; // SDL renderer
    u8 *pixels;             // Graphics buffer (128x128)
    bool running;           // Main loop running flag
    u32 start_time;         // Startup time in milliseconds
    u32 frame_count;        // Frame counter
    u32 last_fps_time;      // Last FPS update time
    f32 current_fps;        // Current FPS
    bool show_fps;          // Whether to show FPS
    u32 frame_start_time;   // Current frame start time
} VM;

VM *vm_create(SDL_Renderer *renderer, bool show_fps);
void vm_destroy(VM *vm);

void vm_load_script(VM *vm, const char *filename);

void vm_process_events(VM *vm);
void vm_update(VM *vm);
void vm_render(VM *vm);
void vm_limit_fps(VM *vm);

#endif