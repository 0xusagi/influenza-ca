#include "viewport_info.h"
#include "config.h"
#include "window.h"
#include "world.h"
#include <sstream>

ViewportInfo::ViewportInfo(Window& window, int width, int height, int x, int y) 
    : renderer(window.renderer) {

    // create the viewport rectangle
    viewport_rect.x = x;
    viewport_rect.y = y;
    viewport_rect.w = width;
    viewport_rect.h = height;

    // load fonts
    LoadFonts();
   
    // create static textures
    CreateStaticTextures();
}

ViewportInfo::~ViewportInfo() {
    TTF_CloseFont(font);

    // destroy textures
    SDL_DestroyTexture(text_healthy_texture);
    SDL_DestroyTexture(text_infected_texture);
    SDL_DestroyTexture(text_dead_texture);
    SDL_DestroyTexture(text_immune_texture);
    SDL_DestroyTexture(text_time_texture);
}

void ViewportInfo::Draw(World& world) {
    SDL_RenderSetViewport(renderer, &viewport_rect);

    // render the static text
    SDL_RenderCopy(renderer, text_healthy_texture, NULL, &text_healthy_rect);
    SDL_RenderCopy(renderer, text_infected_texture, NULL, &text_infected_rect);
    SDL_RenderCopy(renderer, text_dead_texture, NULL, &text_dead_rect);
    SDL_RenderCopy(renderer, text_immune_texture, NULL, &text_immune_rect);
    SDL_RenderCopy(renderer, text_time_texture, NULL, &text_time_rect);

    // initialise the destination rectangle
    // starts with input_x0 and y = 0
    SDL_Rect dest = SDL_Rect();
    dest.x = input_x0;
    dest.y = 0;

    // healthy 
    std::stringstream ss;
    ss.str("");
    ss << world.counts.healthy;
    ss << "(" << world.counts.healthy * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* healthy_texture;
    CreateTextureFromText(ss.str(), &healthy_texture, &dest);
    SDL_RenderCopy(renderer, healthy_texture, NULL, &dest);
    SDL_DestroyTexture(healthy_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << world.counts.infected;
    ss << "(" << world.counts.infected * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* infected_texture;
    CreateTextureFromText(ss.str(), &infected_texture, &dest);
    SDL_RenderCopy(renderer, infected_texture, NULL, &dest);
    SDL_DestroyTexture(infected_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << world.counts.dead;
    ss << "(" << world.counts.dead * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* dead_texture;
    CreateTextureFromText(ss.str(), &dead_texture, &dest);
    SDL_RenderCopy(renderer, dead_texture, NULL, &dest);
    SDL_DestroyTexture(dead_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << world.counts.immune;
    ss << "(" << world.counts.immune * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* immune_texture;
    CreateTextureFromText(ss.str(), &immune_texture, &dest);
    SDL_RenderCopy(renderer, immune_texture, NULL, &dest);
    SDL_DestroyTexture(immune_texture);

    dest.y += dest.h + margin;

    // calculate the current time in hours and mins
    int hours;
    int mins;
    if (kFlowRate < 1) {
        hours = world.timestep * kFlowRate;
        mins = 0;
    }
    else {
        hours = world.timestep / kFlowRate;
        mins = (world.timestep % (int)kFlowRate) * 60 / kFlowRate;
    }
    ss.str("");
    ss << hours << " h" << mins << " m";
    SDL_Texture* time_texture;
    CreateTextureFromText(ss.str(), &time_texture, &dest);
    SDL_RenderCopy(renderer, time_texture, NULL, &dest);
    SDL_DestroyTexture(time_texture);
}

void ViewportInfo::LoadFonts() {
    // load font
    font = TTF_OpenFont("../fonts/calibri.ttf", 14);
    if (font == NULL) {
        SDL_Log("Failed to load calibri font: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }   

    // set font color to black
    font_color.r = 0x00;
    font_color.g = 0x00;
    font_color.b = 0x00;
    font_color.a = 0xFF;
}

void ViewportInfo::CreateStaticTextures() {
    int w;

    // healthy
    CreateTextureFromText("Healthy cells: ", &text_healthy_texture, &text_healthy_rect); 
    text_healthy_rect.x = 0;
    text_healthy_rect.y = 0;
    input_x0 = text_healthy_rect.x + text_healthy_rect.w;

    // infected
    CreateTextureFromText("Infected cells: ", &text_infected_texture, &text_infected_rect);
    text_infected_rect.x = 0;
    text_infected_rect.y = text_healthy_rect.y + text_healthy_rect.h + margin;
    w = text_infected_rect.x + text_infected_rect.w;
    if (w > input_x0) 
        input_x0 = w;

    // dead
    CreateTextureFromText("Dead cells: ", &text_dead_texture, &text_dead_rect);
    text_dead_rect.x = 0;
    text_dead_rect.y = text_infected_rect.y + text_infected_rect.h + margin;
    w = text_dead_rect.x + text_dead_rect.w;
    if (w > input_x0)
        input_x0 = w;

    // immune
    CreateTextureFromText("Immune cells: ", &text_immune_texture, &text_immune_rect);
    text_immune_rect.x = 0;
    text_immune_rect.y = text_dead_rect.y + text_dead_rect.h + margin;
    w = text_immune_rect.x + text_immune_rect.w;
    if (w > input_x0)
        input_x0 = w;

    // time
    CreateTextureFromText("Time: ", &text_time_texture, &text_time_rect);
    text_time_rect.x = 0;
    text_time_rect.y = text_immune_rect.y + text_immune_rect.h + margin;
    w = text_time_rect.x + text_time_rect.w;
    if (w > input_x0)
        input_x0 = w;
}

void ViewportInfo::CreateTextureFromText(std::string text, SDL_Texture** texture, SDL_Rect* rect) {
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
