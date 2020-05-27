#ifndef VIEWPORT_GRID_H
#define VIEWPORT_GRID_H

#include <SDL.h>

class Window;
class World;

class ViewportGrid {
public:
    SDL_Rect viewport_rect;

    ViewportGrid(Window& window, int w, int h);
    ~ViewportGrid();

    void Draw(World& world);

private:
    // current index of the grid
    int x0;
    int y0;

    // maximum number of index which can be 
    // fit onto the current viewport
    int max_x;
    int max_y;

    SDL_Color epi_healthy_color;
    SDL_Color epi_infected_color;
    SDL_Color epi_expressing_color;
    SDL_Color epi_infectious_color;
    SDL_Color epi_dead_color;

    SDL_Texture* immune_cell_img;

    SDL_Renderer* renderer;

    void LoadImages(Window& window);

    SDL_Rect* CreateCellRect(int x, int y);
};

#endif
