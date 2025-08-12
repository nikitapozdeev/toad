#ifndef TOAD_VM_H
#define TOAD_VM_H

#include <SDL.h>
#include "types.h"

// Forward declaration
typedef struct LanguageEngine LanguageEngine;

// VRAM structure containing graphics buffer and palette
typedef struct
{
    u8 *graphics; // Graphics buffer (128x128 pixels)
    u32 *palette; // Color palette (16 colors, RGB format)
    u8 *memory;   // Raw memory block containing both graphics and palette
} VRAM;

typedef struct
{
    SDL_Renderer *renderer; // SDL renderer
    VRAM *vram;             // Video RAM containing graphics buffer and palette
    bool running;           // Main loop running flag
    u32 start_time;         // Startup time in milliseconds
    u32 frame_count;        // Frame counter
    u32 last_fps_time;      // Last FPS update time
    f32 current_fps;        // Current FPS
    bool show_fps;          // Whether to show FPS
    u32 frame_start_time;   // Current frame start time

    // Language engine interface
    LanguageEngine *engine;
} VM;

// VRAM functions
VRAM *vram_create(void);
void vram_destroy(VRAM *vram);
void vram_init_default_palette(VRAM *vram);

// VM functions
VM *vm_create(SDL_Renderer *renderer, bool show_fps);
void vm_destroy(VM *vm);

int vm_load_script(VM *vm, const char *filename);

void vm_process_events(VM *vm);
void vm_update(VM *vm);
void vm_render(VM *vm);
void vm_limit_fps(VM *vm);

#endif