#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <iostream>
#include "cpu.hpp"

#define SCREEN_WIDTH (960)
#define SCREEN_HEIGHT (480)
#define WIDTH (64)
#define HEIGHT (32)

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
    void update(Cpu _cpu);
    void render(Cpu _cpu);
    void clean();
    bool running() { return isRunning; }
};

#endif