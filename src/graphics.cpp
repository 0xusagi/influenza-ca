#include "graphics.h"

void CreateTextureFromText(SDL_Renderer* renderer, std::string text, SDL_Texture** texture, SDL_Rect* rect, TTF_Font* font, SDL_Color font_color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), font_color);
    if (surface == NULL) {
        printf("SDL_ttf unable to render text surface :%s\n", TTF_GetError());
    }
    else {
        // create teture from surface pixels
        *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (*texture == NULL) {
            printf("SDL unable to create texture from rendered text: %s\n", SDL_GetError());
        }

        // create the rect if not already created
        rect->w = surface->w;
        rect->h = surface->h;

        // get rid of old surface
        SDL_FreeSurface(surface);
    }
}
