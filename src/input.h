#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL.h>

class Input {
public:
    std::map<SDL_Keycode, bool> keymap;

    bool quit;
    bool paused;

    Input();

    void Poll();

};

#endif
