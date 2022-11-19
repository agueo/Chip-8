#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <functional>
#include "constants.hpp"
#include "cpu.hpp"

class Window {
public:
    Window(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, Cpu &cpu);
    ~Window();
    void handleRequest();
    void draw(bool can_draw, uint32_t *framebuffer);
    bool running() { return isRunning; }
    void keyDown(SDL_Keycode _key);
    void keyUp(SDL_Keycode _key);

private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_Texture *texture{};
    bool isRunning{};
    Cpu &m_cpu;
};

#endif