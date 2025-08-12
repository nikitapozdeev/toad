#include "vm.h"
#include "engine.h"
#include "defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Default palette (same as original PALETTE from api.c)
static const u32 DEFAULT_PALETTE[PALETTE_SIZE] = {
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

VRAM *vram_create(void)
{
    VRAM *vram = malloc(sizeof(VRAM));
    if (!vram)
        return NULL;

    // Allocate continuous memory block for graphics buffer and palette
    vram->memory = malloc(VRAM_TOTAL_SIZE);
    if (!vram->memory)
    {
        free(vram);
        return NULL;
    }

    // Set up pointers within the memory block
    vram->graphics = vram->memory;
    vram->palette = (u32 *)(vram->memory + VRAM_GRAPHICS_SIZE);

    // Initialize graphics buffer to black (color 0)
    memset(vram->graphics, 0, VRAM_GRAPHICS_SIZE);

    // Initialize palette with default colors
    vram_init_default_palette(vram);

    return vram;
}

void vram_destroy(VRAM *vram)
{
    if (vram)
    {
        if (vram->memory)
        {
            free(vram->memory);
        }
        free(vram);
    }
}

void vram_init_default_palette(VRAM *vram)
{
    if (vram && vram->palette)
    {
        memcpy(vram->palette, DEFAULT_PALETTE, VRAM_PALETTE_SIZE);
    }
}

VM *vm_create(SDL_Renderer *renderer, bool show_fps)
{
    VM *vm = malloc(sizeof(VM));
    if (!vm)
        return NULL;

    vm->renderer = renderer;
    vm->running = true;
    vm->start_time = SDL_GetTicks(); // Remember startup time
    vm->frame_count = 0;
    vm->last_fps_time = vm->start_time;
    vm->current_fps = 0.0f;
    vm->show_fps = show_fps;
    vm->frame_start_time = vm->start_time;

    // Initialize language engine to none
    vm->engine = NULL;

    // Create VRAM
    vm->vram = vram_create();
    if (!vm->vram)
    {
        free(vm);
        return NULL;
    }

    return vm;
}

void vm_destroy(VM *vm)
{
    if (vm)
    {
        // Destroy language engine
        if (vm->engine)
        {
            vm->engine->destroy(vm->engine->engine_data);
            free(vm->engine);
        }

        if (vm->vram)
        {
            vram_destroy(vm->vram);
        }
        free(vm);
    }
}

int vm_load_script(VM *vm, const char *filename)
{
    vm->start_time = SDL_GetTicks();

    // Create engine based on file extension
    vm->engine = create_engine_for_file(vm, filename);
    if (!vm->engine)
    {
        return -1;
    }

    // Load script using engine interface
    if (vm->engine->load_script(vm->engine->engine_data, filename) != 0)
    {
        vm->running = false;
        return -1;
    }

    return 0;
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
    if (vm->engine)
    {
        if (vm->engine->call_update(vm->engine->engine_data) != 0)
        {
            vm->running = false;
        }
    }
}

void vm_render(VM *vm)
{
    // Remember frame start time
    vm->frame_start_time = SDL_GetTicks();

    // Call draw() function using engine interface
    if (vm->engine)
    {
        if (vm->engine->call_draw(vm->engine->engine_data) != 0)
        {
            vm->running = false;
        }
    }

    // Pixel rendering
    SDL_SetRenderDrawColor(vm->renderer, 0, 0, 0, 255);
    SDL_RenderClear(vm->renderer);

    // Graphics buffer rendering
    for (int y = 0; y < SCREEN_H; ++y)
    {
        for (int x = 0; x < SCREEN_W; ++x)
        {
            u8 palette_index = vm->vram->graphics[y * SCREEN_W + x];

            // Limit palette index to 0-15
            palette_index &= 0x0F;

            // Get color from VRAM palette
            u32 color = vm->vram->palette[palette_index];
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