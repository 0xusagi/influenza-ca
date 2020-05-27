#include <SDL.h>
#include <cstdlib>

#include "window.h"
#include "world.h"

int kDefaultWindowWidth = 1200;
int kDefaultWindowHeight = 900;

Window::Window(int on) 
    : on(on)
    , width(kDefaultWindowWidth)
    , height(kDefaultWindowHeight) {

    if (!on) {
        return;
    }

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // create window
    printf("Creating window...\n");
    window = SDL_CreateWindow(
            "Simulation",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Unable to create window SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Unable to create renderer SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

Window::~Window() {
    if (on) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);

        SDL_Quit();
    }
}

void Window::ClearScreen() {
    if (!on) return;

    // clear the renderer and reset the whole screen to white
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

void Window::Draw(World& world) {
    // clear the screen first
    ClearScreen();

    // draw epithelial cells and immune cells to the screen
}
