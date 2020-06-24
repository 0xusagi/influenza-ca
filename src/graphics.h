#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

void CreateTextureFromText(SDL_Renderer* renderer, std::string text, SDL_Texture** texture, SDL_Rect* rect, TTF_Font* font, SDL_Color font_color); 

#endif
