#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class Input;
class World;
class ViewportGrid;
class ViewportInfo;

class Window {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ViewportGrid* viewport_grid;
    ViewportInfo* viewport_info;

    int on; // flag whether graphics is turned on 

    int width;
    int height;

    Window(struct cmd_opts &options);
    ~Window();

    void Draw(World& world);

    int Move(Input& input);

private:
    void ClearScreen();
};

#endif
