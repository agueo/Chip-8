#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <iostream>

class Window 
{
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;

    public:
    Window();
    ~Window();
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleRequest();
    void update();
    void render();
    void clean();
    bool running() {return isRunning; }

};

#endif