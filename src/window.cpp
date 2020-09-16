#include "../include/window.hpp"

Window::Window() {}
Window::~Window() {}

void Window::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    cnt = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // initialize 
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window) {
            std::cout << "Window Created ..." << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer) {
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            std::cout << "Renderer Created ..." << std::endl;
        }

        isRunning = true;
    } else {
        isRunning = false;
    }
}

void Window::handleRequest() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Window::update() {
    std::cout << cnt++ << std::endl;
}

void Window::render() {
    SDL_RenderClear(renderer);
    // place things to render here
    SDL_RenderPresent(renderer);
}

void Window::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "Window destroyed!" << std::endl;
}