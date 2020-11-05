#include <SDL_image.h>

#include "config.h"
#include "epithelial_cell.h"
#include "immune_cell.h"
#include "graphics.h"
#include "viewport_grid.h"
#include "world.h"
#include "window.h"

// cell width and height in pixels
int kCellWidthPixels = 16; 
int kCellHeightPixels = 16;

ViewportGrid::ViewportGrid(Window& window, int viewport_width, int viewport_height, const char* base_name) 
    : renderer(window.renderer)
    , base_name(base_name) {

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

    // set to the value determined by config
    x0 = graphics_start_x;
    y0 = graphics_start_y;

    // handle if too far down so resize 
    if (x0 + max_x >= kGridWidth) x0 = kGridWidth - max_x;
    if (y0 + max_y >= kGridHeight) y0 = kGridHeight - max_y;

    //TODO resize the viewport and screen if the grid size in pixels is smaller
    // than default

    epi_healthy_color = {0xFF, 0xFF, 0xFF, 0xFF};
    epi_infected_color = {0x64, 0xFF, 0x00, 0xFF};
    epi_expressing_color = {0xFF, 0xD5, 0x00, 0xFF};
    epi_infectious_color = {0xFF, 0x00, 0x00, 0xFF};
    epi_dead_color = {0x00, 0x00, 0x00, 0xFF};

    // Load fonts
    LoadFonts();
}

ViewportGrid::~ViewportGrid() {
    SDL_DestroyTexture(immune_cell_img);
}

void ViewportGrid::LoadImages() {
    SDL_Surface* loaded_surface = IMG_Load("images/immune_cell.png");
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

void ViewportGrid::LoadFonts() {
    // Load font
    font = TTF_OpenFont("fonts/calibri.ttf", 12);
    if (font == NULL) {
        SDL_Log("Failed to load calibri font: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // set font color to black
    font_color.r = 0x00;
    font_color.g = 0x00;
    font_color.b = 0x00;
    font_color.a = 0x00;
}

void ViewportGrid::Draw(World& world) {
    SDL_RenderSetViewport(renderer, &viewport_rect);

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
            else if (state == EpithelialState::S_INFECTED ||
                     state == EpithelialState::D_INFECTED || 
                     state == EpithelialState::C_INFECTED) {
                color = epi_infected_color;
            }
            else if (state == EpithelialState::S_EXPRESSING || 
                     state == EpithelialState::C_EXPRESSING) {
                color = epi_expressing_color;
            }
            else if (state == EpithelialState::S_INFECTIOUS || 
                     state == EpithelialState::C_INFECTIOUS) {
                color = epi_infectious_color;
            }
            else if (state == EpithelialState::DEAD) {
                color = epi_dead_color;
            }

            // draw the color and rectangle to screen
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &fill_rect);

            int dip = 0;
            int stv = 0;

            if (state == EpithelialState::S_INFECTED ||
                state == EpithelialState::S_EXPRESSING || 
                state == EpithelialState::S_INFECTIOUS) {
                stv = 1;
            }
            else if (state == EpithelialState::D_INFECTED) {
                dip = 1;
            }
            else if (state == EpithelialState::C_INFECTED || 
                     state == EpithelialState::C_EXPRESSING || 
                     state == EpithelialState::C_INFECTIOUS) {
                stv = 1;
                dip = 1;
            }

            // write the flag for dip and stv particles 
            // stv
            SDL_Rect dest;
            dest.x = fill_rect.x + fill_rect_margin;
            dest.y = fill_rect.y + fill_rect_margin;
            SDL_Texture* stv_count_texture;
            CreateTextureFromText(renderer, std::to_string(stv), &stv_count_texture, &dest, font, font_color);
            SDL_RenderCopy(renderer, stv_count_texture, NULL, &dest);
            SDL_DestroyTexture(stv_count_texture);

            // dip
            dest.x += dest.w + fill_rect_margin;
            SDL_Texture* dip_count_texture;
            CreateTextureFromText(renderer, std::to_string(dip), &dip_count_texture, &dest, font, font_color);
            SDL_RenderCopy(renderer, dip_count_texture, NULL, &dest);
            SDL_DestroyTexture(dip_count_texture);
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

    // save screenshot every 12 hours
    int is_12h = world.timestep % (int)(kFlowRate * 12);
    if (is_12h == 0) {
        SaveScreenshot(world);
    }
}

void ViewportGrid::SaveScreenshot(World &world) {
    SDL_Rect _viewport;
    SDL_RenderGetViewport(renderer, &_viewport);

    SDL_Surface *sshot = SDL_CreateRGBSurface(0, viewport_rect.w, viewport_rect.h, 32, 0, 0, 0, 0);

    if (sshot == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        return;
    }

    if (SDL_RenderReadPixels(renderer, &viewport_rect, sshot->format->format, sshot->pixels, sshot->pitch) != 0) {
        SDL_Log("SDL_RenderReadPixels() failed: %s\n", SDL_GetError());
    }
    else {

        int hour = world.timestep / kFlowRate;
        std::string sshot_name = base_name + "-" + std::to_string(hour) + ".png";

        if (IMG_SavePNG(sshot, sshot_name.c_str()) != 0) {
            printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        }
    }
    SDL_FreeSurface(sshot);
}

void ViewportGrid::RepositionDown() {
    if (y0 < kGridHeight - 2 - max_y) {
        y0++;
    }
}

void ViewportGrid::RepositionUp() {
    if (y0 > 0) {
        y0--;
    }
}

void ViewportGrid::RepositionLeft() {
    if (x0 > 0) {
        x0--;
    }
}

void ViewportGrid::RepositionRight() {
    if (x0 < kGridWidth - 2 - max_x) {
        x0++;
    }
}

void ViewportGrid::AssignToCellRect(int x, int y, SDL_Rect* rect) {
    rect->x = (x - x0) * kCellWidthPixels;
    rect->y = (y - y0) * kCellHeightPixels;
    rect->w = kCellWidthPixels;
    rect->h = kCellHeightPixels;
}
