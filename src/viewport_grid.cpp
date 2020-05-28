#include <SDL_image.h>

#include "config.h"
#include "epithelial_cell.h"
#include "immune_cell.h"
#include "viewport_grid.h"
#include "world.h"
#include "window.h"

// cell width and height in pixels
int kCellWidthPixels = 16; 
int kCellHeightPixels = 16;

ViewportGrid::ViewportGrid(Window& window, int viewport_width, int viewport_height) 
    : renderer(window.renderer) {

    // create the viewport rectangle 
    viewport_rect.x = 0;
    viewport_rect.y = 0;
    viewport_rect.w = viewport_width;
    viewport_rect.h = viewport_height;
    
    // load images
    LoadImages();

    // calculate the number of cells which can be fit
    max_x = viewport_rect.w / kCellWidthPixels;
    max_y = viewport_rect.h / kCellHeightPixels;

    // set to the middle of the grid
    x0 = kGridWidth / 2 - max_x / 2;
    y0 = kGridHeight / 2 - max_y / 2;

    //TODO resize the viewport and screen if the grid size in pixels is smaller
    // than default

    epi_healthy_color = {0xFF, 0xFF, 0xFF, 0xFF};
    epi_infected_color = {0x64, 0xFF, 0x00, 0xFF};
    epi_expressing_color = {0xFF, 0xD5, 0x00, 0xFF};
    epi_infectious_color = {0xFF, 0x00, 0x00, 0xFF};
    epi_dead_color = {0x00, 0x00, 0x00, 0xFF};
}

ViewportGrid::~ViewportGrid() {
    SDL_DestroyTexture(immune_cell_img);
}

void ViewportGrid::LoadImages() {
    SDL_Surface* loaded_surface = IMG_Load("../images/immune_cell.png");
    if (loaded_surface == NULL) {
        SDL_Log("Unable to load immune cell image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    else {
        immune_cell_img = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (immune_cell_img == NULL) {
            SDL_Log("Unable to create immune cell texture from image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        // get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }
}

void ViewportGrid::Draw(World& world) {
    for (int i = 0; i < max_x; i++) {
        for (int j = 0; j < max_y; j++) {
            int x = i + x0;
            int y = j + y0;

            SDL_Rect fill_rect;
            AssignToCellRect(x, y, &fill_rect);

            // pick the color
            EpithelialState state = world.epithelial_cells[x][y]->state;
            SDL_Color color;
            if (state == EpithelialState::HEALTHY) {
                color = epi_healthy_color;
            }
            else if (state == EpithelialState::INFECTED) {
                color = epi_infected_color;
            }
            else if (state == EpithelialState::EXPRESSING) {
                color = epi_expressing_color;
            }
            else if (state == EpithelialState::INFECTIOUS) {
                color = epi_infectious_color;
            }
            else if (state == EpithelialState::DEAD) {
                color = epi_dead_color;
            }

            // draw the color and rectangle to screen
            SDL_RenderSetViewport(renderer, &viewport_rect);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &fill_rect);
        }
    }

    for (ImmuneCell cell : world.immune_cells) {
        if ((cell.x >= x0 || cell.x < max_x) && (cell.y >= y0 || cell.y < max_y)) {
            SDL_Rect fill_rect;
            AssignToCellRect(cell.x, cell.y, &fill_rect);

            // render the immune cell
            SDL_RenderCopy(renderer, immune_cell_img, NULL, &fill_rect);
        }
    }
}

void ViewportGrid::AssignToCellRect(int x, int y, SDL_Rect* rect) {
    rect->x = (x - x0) * kCellWidthPixels;
    rect->y = (y - y0) * kCellHeightPixels;
    rect->w = kCellWidthPixels;
    rect->h = kCellHeightPixels;
}
