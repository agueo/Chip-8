#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <iostream>
#include "constants.hpp"
#include "cpu.hpp"

class Window 
{
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool isRunning;
    Cpu *m_cpu;

    public:
    Window();
    ~Window();
    int init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    void connect_cpu(Cpu *c) { m_cpu = c; }
    void handleRequest();
    void draw(bool can_draw, uint32_t *framebuffer);
    void clean();
    bool running() { return isRunning; }
    void keyDown(SDL_Keycode _key);
    void keyUp(SDL_Keycode _key);
};

#endif