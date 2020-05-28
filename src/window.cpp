#include <SDL.h>
#include <cstdlib>

#include "viewport_grid.h"
#include "window.h"
#include "world.h"

int kDefaultWindowWidth = 1200;
int kDefaultWindowHeight = 900;

int kViewportGridRatio = 3;
int kViewportInfoRatio = 1;
int kTotalRatio = kViewportGridRatio + kViewportInfoRatio;

Window::Window(int on) 
    : on(on)
    , width(kDefaultWindowWidth)
    , height(kDefaultWindowHeight) {

    if (!on) {
        return;
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

    // create viewports
    int viewport_grid_width = width * kViewportGridRatio / kTotalRatio;
    int viewport_grid_height = height;
    viewport_grid = new ViewportGrid(*this, viewport_grid_width, viewport_grid_height);
}

Window::~Window() {
    if (on) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);

        SDL_Quit();

        // destroy viewports
        delete viewport_grid;
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

    // draw epithelial cells and immune cells to buffer
    viewport_grid->Draw(world);

    // draw to the screen
    SDL_RenderPresent(renderer);
}
