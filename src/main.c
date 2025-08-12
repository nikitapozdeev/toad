#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    if (argc < 3 || strcmp(argv[1], "-p") != 0)
    {
        printf("Usage: toad -p <path to game> [--fps]\n");
        return 1;
    }

    // Check --fps flag
    bool show_fps = false;
    char *script_path = argv[2];

    if (argc >= 4 && strcmp(argv[3], "--fps") == 0)
    {
        show_fps = true;
    }

    char title[64];
    snprintf(title, sizeof(title), "Toad v.%s", VERSION);

    // SDL initialization
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W * SCALE, SCREEN_H * SCALE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    VM *vm = vm_create(renderer, show_fps);
    vm_load_script(vm, script_path);

    while (vm->running)
    {
        vm_process_events(vm);
        vm_update(vm);
        vm_render(vm);
        vm_limit_fps(vm);
    }

    vm_destroy(vm);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}