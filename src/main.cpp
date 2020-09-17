/*
 * Chip-8 Emulator on Linux
 *
 * Author: Agueo Lopez
 */

#include "../include/window.hpp"
#include "../include/cpu.hpp"

Window *window = nullptr;
Cpu *cpu = nullptr;

int main(int argc, char *argv[]){
    // TODO - fix FPS for 60Hz f = 1/t -> t = 1/f == 1/(60)
    // const int FPS = 60;
    // const int frameDelay = 1000 / FPS;
    // Uint32 frameStart;
    // Uint32 frameTime;

    // Get ROM name
    if(argc != 2) {
        std::cout << "usage\n ./chip_8 <program>" << std::endl;
    }

    // Create CPU
    cpu = new Cpu();
    if(!cpu->loadRom(argv[1]))
    {
        return 1;
    }

    while(!cpu->isHalted()) {
        // cpu->disassemble_program();
        cpu->fetch();
        cpu->decode_execute();

        if(cpu->debugPC() >= 4096){
            cpu->setHaltCpu();
        }
    }
    /*
    * window = new Window();
    * window->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 250, false);

    * while(window->running()) {
    *     // get the time since window was created
    *     frameStart = SDL_GetTicks();

    *     // handle window changes, updates, and render
    *     window->handleRequest();        
    *     window->update();        
    *     window->render();

    *     // time it took to handle requests
    *     frameTime = SDL_GetTicks() - frameStart;
    *     if(frameDelay > frameTime) {
    *         SDL_Delay(frameDelay - frameTime);
    *     }
    * }

    * window->clean();
    */


    return 0;
}