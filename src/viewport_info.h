#ifndef VIEWPORT_INFO_H
#define VIEWPORT_INFO_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Window;
class World;

class ViewportInfo {
public:
    SDL_Rect viewport_rect;

    ViewportInfo(Window& window, int width, int height, int x, int y);
    ~ViewportInfo();

    void Draw(World& world);

private:
    const int margin = 5;

    SDL_Renderer* renderer;

    TTF_Font* font;
    SDL_Color font_color;

    SDL_Texture* text_healthy_texture;
    SDL_Texture* text_stv_infected_texture;
    SDL_Texture* text_dip_infected_texture;
    SDL_Texture* text_co_infected_texture;
    SDL_Texture* text_dead_texture;
    SDL_Texture* text_immune_texture;
    SDL_Texture* text_time_texture;

    SDL_Rect text_healthy_rect;
    SDL_Rect text_stv_infected_rect;
    SDL_Rect text_dip_infected_rect;
    SDL_Rect text_co_infected_rect;
    SDL_Rect text_dead_rect;
    SDL_Rect text_immune_rect;
    SDL_Rect text_time_rect;

    int input_x0; // the starting point for dynamic input on RHS

    void LoadFonts();
    void CreateStaticTextures();
};

#endif
