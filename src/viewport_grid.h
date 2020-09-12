#ifndef VIEWPORT_GRID_H
#define VIEWPORT_GRID_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Window;
class World;

class ViewportGrid {
public:
    SDL_Rect viewport_rect;

    std::string base_name;

    ViewportGrid(Window& window, int w, int h, const char* base_name);
    ~ViewportGrid();

    void Draw(World& world);

    void SaveScreenshot(World& world);

    void RepositionDown();
    void RepositionUp();
    void RepositionLeft();
    void RepositionRight();

private:
    const int fill_rect_margin = 2;

    TTF_Font* font;
    SDL_Color font_color;

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

    void LoadImages();
    void LoadFonts();

    void AssignToCellRect(int x, int y, SDL_Rect* rect);
};

#endif
