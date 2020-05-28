#include "input.h"

Input::Input() {
    quit = false;
}

void Input::Poll() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        // quit with x on window
        if (event.type == SDL_QUIT) {
            quit = true;
        }

        else if (event.type == SDL_KEYDOWN) {
            keymap[event.key.keysym.sym] = true;

            // quit if escape key is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }

            // pause
            bool space_pressed = keymap[SDLK_SPACE];
            if (space_pressed) {
                paused = !paused;
            }
        }
        
        else if (event.type == SDL_KEYUP) {
            keymap[event.key.keysym.sym] = false;
       }
    }
}
