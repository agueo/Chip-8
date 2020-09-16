#include "../include/window.hpp"

Window *window = nullptr;

int main(int argc, char *argv[]){
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    window = new Window();
    window->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 250, false);

    while(window->running()) {
        // get the time since window was created
        frameStart = SDL_GetTicks();

        // handle window changes, updates, and render
        window->handleRequests();        
        window->update();        
        window->render();

        // time it took to handle requests
        frameTime = SDL_GetTicks - frameStart;
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime)
        }
    }


    std::cout << argc << " " << argv[0] << std::endl;


    return 0;
}