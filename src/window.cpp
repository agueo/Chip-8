#include "../include/window.hpp"

Window::Window()
{
    window = nullptr;
    renderer = nullptr;
    texture = nullptr;
    isRunning = false;
}

Window::~Window() {}

int Window::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // initialize 
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(!window) return 1;
        std::cout << "Window Created ..." << std::endl;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) return 1;
        std::cout << "Renderer Created ..." << std::endl;

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
        if(!texture) return 1;
        std::cout << "Texture Created... " << std::endl;

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        std::cout << "setting run flag..." << std::endl;
        isRunning = true;
    }
    return 0;
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

void Window::draw(Cpu _cpu) {
    if(_cpu.canDraw()) {
       SDL_RenderClear(renderer);
       SDL_UpdateTexture(texture, NULL, _cpu.frame_buffer, WIDTH * sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

void Window::clean() {
    SDL_DestroyTexture(texture);
    std::cout << "Texture destroyed!" << std::endl;
    SDL_DestroyRenderer(renderer);
    std::cout << "Renderer destroyed!" << std::endl;
    SDL_DestroyWindow(window);
    std::cout << "Window destroyed!" << std::endl;
}