#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class World;
class ViewportGrid;

class Window {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ViewportGrid* viewport_grid;

    int on; // flag whether graphics is turned on 

    int width;
    int height;

    Window(int on);
    ~Window();

    void Draw(World& world);

private:
    void ClearScreen();
};

#endif
