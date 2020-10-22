#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>

#include "config.h"
#include "input.h"
#include "utils.h"
#include "viewport_info.h"
#include "viewport_grid.h"
#include "window.h"
#include "world.h"

int kDefaultWindowWidth = 1200;
int kDefaultWindowHeight = 900;

int kViewportGridRatio = 3;
int kViewportInfoRatio = 1;
int kTotalRatio = kViewportGridRatio + kViewportInfoRatio;

Window::Window(struct cmd_opts &options) 
    : on(options.graphics)
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

    // init SDL image
    if (!IMG_Init(IMG_INIT_PNG)) {
        SDL_Log("Unable to initialise SDL image: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // init SDL ttf
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not be initialised: %s\n", TTF_GetError());
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

    // create viewports
    int viewport_grid_width = width * kViewportGridRatio / kTotalRatio;
    int viewport_grid_height = height;
    viewport_grid = new ViewportGrid(*this, viewport_grid_width, viewport_grid_height, options.base_name);

    int viewport_info_width = width * kViewportInfoRatio / kTotalRatio;
    int viewport_info_height = height;
    viewport_info = new ViewportInfo(
            *this, 
            viewport_info_width, 
            viewport_info_height, 
            viewport_grid_width, 
            0);
}

Window::~Window() {
    if (on) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);

        // destroy viewports
        delete viewport_grid;
        delete viewport_info;

        IMG_Quit();
        TTF_Quit();
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
    if (!on) return;

    // clear the screen first
    ClearScreen();

    // draw epithelial cells and immune cells to buffer
    viewport_grid->Draw(world);
    viewport_info->Draw(world);

    // draw to the screen
    SDL_RenderPresent(renderer);

    // save screenshot every 12 hours
    //int is_12h = world.timestep % (int)(kFlowRate * 12);
    //if (is_12h == 0) {
    //    viewport_grid->SaveScreenshot(world);
    //}
}

int Window::Move(Input& input) {
    int moved = 0;

    // any movement is related to the grid viewport
    // move down
    if (input.IsDown(Keyboard::Down)) {
        viewport_grid->RepositionDown();
        moved = 1;
    }
    // move left
    if (input.IsDown(Keyboard::Left)) {
        viewport_grid->RepositionLeft();
        moved = 1;
    }
    // move up
    if (input.IsDown(Keyboard::Up)) {
        viewport_grid->RepositionUp();
        moved = 1;
    }
    // move right
    if (input.IsDown(Keyboard::Right)) {
        viewport_grid->RepositionRight();
        moved = 1;
    }

    return moved;
}
