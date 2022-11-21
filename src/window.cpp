#include "window.hpp"

Window::Window(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, Cpu &cpu)
    : window{nullptr}, renderer{nullptr}, texture{nullptr}, isRunning{false}, m_cpu{cpu}
{
    int flags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // initialize 
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(!window) return;
        SDL_Log("Window created...");

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) return;
        SDL_Log("Renderer created...");

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
        SDL_Log("Texture created...");

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Log("Setting run flag");
        isRunning = true;
    }
}

Window::~Window() noexcept { 
    SDL_DestroyTexture(texture);
    std::cout << "Texture destroyed!" << std::endl;
    SDL_DestroyRenderer(renderer);
    std::cout << "Renderer destroyed!" << std::endl;
    SDL_DestroyWindow(window);
    std::cout << "Window destroyed!" << std::endl;
    SDL_Quit();
}

void Window::handleRequest() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            keyDown(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            keyUp(event.key.keysym.sym);
            break;
        default:
            break;
    }
}

void Window::keyDown(SDL_Keycode _key) {
    switch(_key) {
        case SDLK_1: m_cpu.key_state[0x1] = 1; break;
        case SDLK_2: m_cpu.key_state[0x2] = 1; break;
        case SDLK_3: m_cpu.key_state[0x3] = 1; break;
        case SDLK_4: m_cpu.key_state[0xc] = 1; break;
        case SDLK_q: m_cpu.key_state[0x4] = 1; break;
        case SDLK_w: m_cpu.key_state[0x5] = 1; break;
        case SDLK_e: m_cpu.key_state[0x6] = 1; break;
        case SDLK_r: m_cpu.key_state[0xd] = 1; break;
        case SDLK_a: m_cpu.key_state[0x7] = 1; break;
        case SDLK_s: m_cpu.key_state[0x8] = 1; break;
        case SDLK_d: m_cpu.key_state[0x9] = 1; break;
        case SDLK_f: m_cpu.key_state[0xe] = 1; break;
        case SDLK_z: m_cpu.key_state[0xa] = 1; break;
        case SDLK_x: m_cpu.key_state[0x0] = 1; break;
        case SDLK_c: m_cpu.key_state[0xb] = 1; break;
        case SDLK_v: m_cpu.key_state[0xf] = 1; break;
    }
}

void Window::keyUp(SDL_Keycode _key) {
    switch(_key) {
        case SDLK_1: m_cpu.key_state[0x1] = 0; break;
        case SDLK_2: m_cpu.key_state[0x2] = 0; break;
        case SDLK_3: m_cpu.key_state[0x3] = 0; break;
        case SDLK_4: m_cpu.key_state[0xc] = 0; break;
        case SDLK_q: m_cpu.key_state[0x4] = 0; break;
        case SDLK_w: m_cpu.key_state[0x5] = 0; break;
        case SDLK_e: m_cpu.key_state[0x6] = 0; break;
        case SDLK_r: m_cpu.key_state[0xd] = 0; break;
        case SDLK_a: m_cpu.key_state[0x7] = 0; break;
        case SDLK_s: m_cpu.key_state[0x8] = 0; break;
        case SDLK_d: m_cpu.key_state[0x9] = 0; break;
        case SDLK_f: m_cpu.key_state[0xe] = 0; break;
        case SDLK_z: m_cpu.key_state[0xa] = 0; break;
        case SDLK_x: m_cpu.key_state[0x0] = 0; break;
        case SDLK_c: m_cpu.key_state[0xb] = 0; break;
        case SDLK_v: m_cpu.key_state[0xf] = 0; break;
    }
}

void Window::draw(bool can_draw, uint32_t *framebuffer) {
    if(!can_draw) return;

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}