#ifndef TOAD_DEFS_H
#define TOAD_DEFS_H

// Console configuration
#define SCREEN_W 128
#define SCREEN_H 128
#define PALETTE_SIZE 16
#define SCALE 3

// VRAM configuration
#define VRAM_GRAPHICS_SIZE (SCREEN_W * SCREEN_H)
#define VRAM_PALETTE_SIZE (PALETTE_SIZE * sizeof(u32))
#define VRAM_TOTAL_SIZE (VRAM_GRAPHICS_SIZE + VRAM_PALETTE_SIZE)

// FPS configuration
#define TARGET_FPS 60
#define FRAME_TIME_MS (1000 / TARGET_FPS)

// Version
#define VERSION "0.1.0"

#endif