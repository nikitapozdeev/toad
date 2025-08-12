#include "vm.h"
#include "api.h"
#include "defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

VM *vm_create(SDL_Renderer *renderer, bool show_fps)
{
    VM *vm = malloc(sizeof(VM));
    vm->renderer = renderer;
    vm->running = true;
    vm->start_time = SDL_GetTicks(); // Remember startup time
    vm->frame_count = 0;
    vm->last_fps_time = vm->start_time;
    vm->current_fps = 0.0f;
    vm->show_fps = show_fps;
    vm->frame_start_time = vm->start_time;

    // Lua initialization
    vm->L = luaL_newstate();
    luaL_openlibs(vm->L);

    // API registration
    api_register(vm->L, vm);

    // Graphics buffer
    vm->pixels = malloc(SCREEN_W * SCREEN_H * sizeof(u8));
    memset(vm->pixels, 0, SCREEN_W * SCREEN_H);

    return vm;
}

void vm_destroy(VM *vm)
{
    if (vm)
    {
        if (vm->L)
        {
            lua_close(vm->L);
        }
        if (vm->pixels)
        {
            free(vm->pixels);
        }
        free(vm);
    }
}

void vm_load_script(VM *vm, const char *filename)
{
    vm->start_time = SDL_GetTicks();

    if (luaL_dofile(vm->L, filename) != LUA_OK)
    {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(vm->L, -1));
        vm->running = false;
    }
}

void vm_process_events(VM *vm)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            vm->running = false;
        }
    }
}

void vm_update(VM *vm)
{
    lua_getglobal(vm->L, "update");
    if (lua_isfunction(vm->L, -1))
    {
        if (lua_pcall(vm->L, 0, 0, 0) != LUA_OK)
        {
            fprintf(stderr, "Error in update(): %s\n", lua_tostring(vm->L, -1));
            vm->running = false;
        }
    }
    else
    {
        lua_pop(vm->L, 1);
    }
}

void vm_render(VM *vm)
{
    // Remember frame start time
    vm->frame_start_time = SDL_GetTicks();

    // Call draw() in Lua
    lua_getglobal(vm->L, "draw");
    if (lua_isfunction(vm->L, -1))
    {
        if (lua_pcall(vm->L, 0, 0, 0) != LUA_OK)
        {
            fprintf(stderr, "Error in draw(): %s\n", lua_tostring(vm->L, -1));
            vm->running = false;
        }
    }
    else
    {
        lua_pop(vm->L, 1);
    }

    // Pixel rendering
    SDL_SetRenderDrawColor(vm->renderer, 0, 0, 0, 255);
    SDL_RenderClear(vm->renderer);

    // Graphics buffer rendering
    for (int y = 0; y < SCREEN_H; ++y)
    {
        for (int x = 0; x < SCREEN_W; ++x)
        {
            u8 palette_index = vm->pixels[y * SCREEN_W + x];

            // Limit palette index to 0-15
            palette_index &= 0x0F;

            // Get color from palette
            u32 color = PALETTE[palette_index];
            u8 r = (color >> 16) & 0xFF;
            u8 g = (color >> 8) & 0xFF;
            u8 b = color & 0xFF;

            SDL_SetRenderDrawColor(vm->renderer, r, g, b, 255);

            // Draw pixel with scaling
            SDL_Rect rect = {x, y, 1, 1};
            SDL_RenderFillRect(vm->renderer, &rect);
        }
    }

    SDL_RenderPresent(vm->renderer);

    // FPS counter update
    vm->frame_count++;
    u32 current_time = SDL_GetTicks();
    if (current_time - vm->last_fps_time >= 1000) // Update FPS every second
    {
        vm->current_fps = vm->frame_count * 1000.0f / (current_time - vm->last_fps_time);
        vm->frame_count = 0;
        vm->last_fps_time = current_time;
        if (vm->show_fps)
        {
            printf("FPS: %.1f\n", vm->current_fps);
        }
    }
}

void vm_limit_fps(VM *vm)
{
    u32 frame_end_time = SDL_GetTicks();
    u32 frame_duration = frame_end_time - vm->frame_start_time;

    if (frame_duration < FRAME_TIME_MS)
    {
        SDL_Delay(FRAME_TIME_MS - frame_duration);
    }
}