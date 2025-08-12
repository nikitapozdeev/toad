#include "api.h"
#include "defs.h"
#include <string.h>
#include <math.h>

void api_putpixel(VM *vm, int x, int y, int color)
{
    if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H && color >= 0 && color < PALETTE_SIZE)
    {
        vm->vram->graphics[y * SCREEN_W + x] = color;
    }
}

// Returns the cosine of angle
double api_cos(double angle)
{
    // In PICO-8 full rotation = 1, not 2π
    // Convert from PICO-8 format to radians
    double radians = angle * 2.0 * M_PI;
    return cos(radians);
}

// Returns the sin of angle
double api_sin(double angle)
{
    // In PICO-8 sin(x) = cos(x - 0.25) (shift by -90 degrees)
    // Convert from PICO-8 format to radians
    double radians = (angle - 0.25) * 2.0 * M_PI;
    return cos(radians);
}

// Returns the square root of number
double api_sqrt(double number)
{
    return sqrt(number);
}

// Clear graphics buffer in VRAM
void api_cls(VM *vm)
{
    memset(vm->vram->graphics, 0, VRAM_GRAPHICS_SIZE);
}

// Returns time in seconds since startup
double api_time(VM *vm)
{
    u32 current_time = SDL_GetTicks();
    return (current_time - vm->start_time) / 1000.0;
}
