#include "viewport_info.h"
#include "config.h"
#include "graphics.h"
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
    SDL_DestroyTexture(text_stv_infected_texture);
    SDL_DestroyTexture(text_dip_infected_texture);
    SDL_DestroyTexture(text_co_infected_texture);
    SDL_DestroyTexture(text_dead_texture);
    SDL_DestroyTexture(text_immune_texture);
    SDL_DestroyTexture(text_time_texture);
}

void ViewportInfo::Draw(World& world) {
    SDL_RenderSetViewport(renderer, &viewport_rect);

    // render the static text
    SDL_RenderCopy(renderer, text_healthy_texture, NULL, &text_healthy_rect);
    SDL_RenderCopy(renderer, text_stv_infected_texture, NULL, &text_stv_infected_rect);
    SDL_RenderCopy(renderer, text_dip_infected_texture, NULL, &text_dip_infected_rect);
    SDL_RenderCopy(renderer, text_co_infected_texture, NULL, &text_co_infected_rect);
    SDL_RenderCopy(renderer, text_dead_texture, NULL, &text_dead_rect);
    SDL_RenderCopy(renderer, text_immune_texture, NULL, &text_immune_rect);
    SDL_RenderCopy(renderer, text_time_texture, NULL, &text_time_rect);

    // initialise the destination rectangle
    // starts with input_x0 and y = 0
    SDL_Rect dest = SDL_Rect();
    dest.x = input_x0;
    dest.y = 0;

    int n_healthy = 0;
    int n_s_infected = 0;
    int n_d_infected = 0;
    int n_c_infected = 0;
    int n_dead = 0;
    int n_immune = 0;

    for (int i = 0; i < kNumSections; i++) {
        n_healthy += world.counts[i].healthy;
        n_s_infected += world.counts[i].s_infected;
        n_d_infected += world.counts[i].d_infected;
        n_c_infected += world.counts[i].c_infected;
        n_dead += world.counts[i].dead;
        n_immune += world.counts[i].immune;
    }

    // healthy 
    std::stringstream ss;
    ss.str("");
    ss << n_healthy;
    ss << "(" << n_healthy * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* healthy_texture;
    CreateTextureFromText(renderer, ss.str(), &healthy_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, healthy_texture, NULL, &dest);
    SDL_DestroyTexture(healthy_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << n_s_infected;
    ss << "(" << n_s_infected * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* stv_infected_texture;
    CreateTextureFromText(renderer, ss.str(), &stv_infected_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, stv_infected_texture, NULL, &dest);
    SDL_DestroyTexture(stv_infected_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << n_d_infected;
    ss << "(" << n_d_infected * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* dip_infected_texture;
    CreateTextureFromText(renderer, ss.str(), &dip_infected_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, dip_infected_texture, NULL, &dest);
    SDL_DestroyTexture(dip_infected_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << n_c_infected;
    ss << "(" << n_c_infected * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* co_infected_texture;
    CreateTextureFromText(renderer, ss.str(), &co_infected_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, co_infected_texture, NULL, &dest);
    SDL_DestroyTexture(co_infected_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << n_dead;
    ss << "(" << n_dead * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* dead_texture;
    CreateTextureFromText(renderer, ss.str(), &dead_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, dead_texture, NULL, &dest);
    SDL_DestroyTexture(dead_texture);

    dest.y += dest.h + margin;

    ss.str("");
    ss << n_immune;
    ss << "(" << n_immune * 1.0 / kTotalEpithelialCells << ")";
    SDL_Texture* immune_texture;
    CreateTextureFromText(renderer, ss.str(), &immune_texture, &dest, font, font_color);
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
    CreateTextureFromText(renderer, ss.str(), &time_texture, &dest, font, font_color);
    SDL_RenderCopy(renderer, time_texture, NULL, &dest);
    SDL_DestroyTexture(time_texture);
}

void ViewportInfo::LoadFonts() {
    // load font
    font = TTF_OpenFont("fonts/calibri.ttf", 14);
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
    CreateTextureFromText(renderer, "Healthy cells: ", &text_healthy_texture, &text_healthy_rect, font, font_color); 
    text_healthy_rect.x = 0;
    text_healthy_rect.y = 0;
    input_x0 = text_healthy_rect.x + text_healthy_rect.w;

    // stv-infected
    CreateTextureFromText(renderer, "STV-infected cells: ", &text_stv_infected_texture, &text_stv_infected_rect, font, font_color);
    text_stv_infected_rect.x = 0;
    text_stv_infected_rect.y = text_healthy_rect.y + text_healthy_rect.h + margin;
    w = text_stv_infected_rect.x + text_stv_infected_rect.w;
    if (w > input_x0) 
        input_x0 = w;

    // dip-infected
     CreateTextureFromText(renderer, "DIP-infected cells: ", &text_dip_infected_texture, &text_dip_infected_rect, font, font_color);
    text_dip_infected_rect.x = 0;
    text_dip_infected_rect.y = text_stv_infected_rect.y + text_stv_infected_rect.h + margin;
    w = text_dip_infected_rect.x + text_dip_infected_rect.w;
    if (w > input_x0) 
        input_x0 = w;

    // co-infected
    CreateTextureFromText(renderer, "Co-infected cells: ", &text_co_infected_texture, &text_co_infected_rect, font, font_color);
    text_co_infected_rect.x = 0;
    text_co_infected_rect.y = text_dip_infected_rect.y + text_dip_infected_rect.h + margin;
    w = text_co_infected_rect.x + text_co_infected_rect.w;
    if (w > input_x0) 
        input_x0 = w;

    // dead
    CreateTextureFromText(renderer, "Dead cells: ", &text_dead_texture, &text_dead_rect, font, font_color);
    text_dead_rect.x = 0;
    text_dead_rect.y = text_co_infected_rect.y + text_co_infected_rect.h + margin;
    w = text_dead_rect.x + text_dead_rect.w;
    if (w > input_x0)
        input_x0 = w;

    // immune
    CreateTextureFromText(renderer, "Immune cells: ", &text_immune_texture, &text_immune_rect, font, font_color);
    text_immune_rect.x = 0;
    text_immune_rect.y = text_dead_rect.y + text_dead_rect.h + margin;
    w = text_immune_rect.x + text_immune_rect.w;
    if (w > input_x0)
        input_x0 = w;

    // time
    CreateTextureFromText(renderer, "Time: ", &text_time_texture, &text_time_rect, font, font_color);
    text_time_rect.x = 0;
    text_time_rect.y = text_immune_rect.y + text_immune_rect.h + margin;
    w = text_time_rect.x + text_time_rect.w;
    if (w > input_x0)
        input_x0 = w;
}

