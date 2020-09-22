/*
 * Chip-8 Emulator on Linux
 *
 * Author: Agueo Lopez
 */

#include "../include/window.hpp"
#include "../include/cpu.hpp"

// Could change to local var
Window *window = nullptr;
Cpu *cpu = nullptr;

int main(int argc, char *argv[]){
    const int FPS = 180;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    Uint32 frameTime;

    // Get ROM name
    if(argc != 2) {
        std::cout << "usage\n ./chip_8 <program>" << std::endl;
        return(1);
    } else {
        std::cout << argv[1] << std::endl;
    }

    std::cout << "Creating window..." << std::endl;

    window = new Window();
    if(window->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false)){
        std::cout << "Failed to initialize window";
        exit(EXIT_FAILURE);
    }

    // Create CPU
    cpu = new Cpu();
    window->connect_cpu(cpu);

    if(cpu->loadRom(argv[1]) == 0)
    {
        std::cout << "Failed to load ROM" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(window->running()) {
        // get the time since window was created
        frameStart = SDL_GetTicks();

        // Do CPU cycle
        cpu->fetch();
        cpu->decode_execute(DISASSEMBLE);

        // handle window changes, updates, and render
        window->handleRequest();        

        // Handle screen updates
        window->draw(cpu->canDraw(), cpu->frame_buffer);

        // reset draw flag
        cpu->setDrawFlag(false);
        
        // handle updating timers 
        cpu->decrementTimers();

        // time it took to handle requests
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    window->clean();

    return 0;
}