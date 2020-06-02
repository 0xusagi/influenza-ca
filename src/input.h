#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL.h>

enum class Keyboard {
    Up = SDLK_UP,
    Down = SDLK_DOWN,
    Left = SDLK_LEFT,
    Right = SDLK_RIGHT,
    Space = SDLK_SPACE,

    s = SDLK_s,
    n = SDLK_n
};

class Input {
public:
    std::map<SDL_Keycode, bool> keymap;

    bool quit;
    bool paused;

    Input();

    void Poll();

    int IsDown(Keyboard key);
    int IsUp(Keyboard key);

};

#endif
