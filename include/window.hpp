#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <iostream>
#include "constants.hpp"

class Window 
{
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool isRunning;

    public:
    Window();
    ~Window();
    int init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleRequest();
    void draw(bool can_draw, uint32_t *framebuffer);
    void clean();
    bool running() { return isRunning; }
};

#endif