#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class World;

class Window {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int on; // flag whether graphics is turned on 

    int width;
    int height;

    Window(int on);
    ~Window();

    void ClearScreen();
    void Draw(World& world);

private:
};

#endif
