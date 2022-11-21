/*
 * Chip-8 Emulator on Linux
 *
 * Author: Agueo Lopez
 */
#include "audio_driver.h"
#include "cpu.hpp"
#include "window.hpp"

#include <memory>

int main(int argc, char *argv[]){
    // Get ROM name
    if(argc != 2) {
        std::cout << "usage\n ./chip_8 <program>" << std::endl;
        return(1);
    } 

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    Uint32 frameTime;

    // Create CPU
    Cpu cpu{};
    AudioDriver audio_driver{};

    // TODO look into this relationship and see if we can keep them separate
    std::cout << "Creating window..." << std::endl;
    Window window{argv[1], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false, cpu};
     
    // load rom
    if(cpu.loadRom(argv[1]) == 0)
    {
        std::cout << "Failed to load ROM" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(window.running()) {
        // get the time since window was created
        frameStart = SDL_GetTicks();

        // handle updating timers 
        cpu.decrementTimers();

        // handle window changes, updates, and render
        window.handleRequest();        

        // Do CPU cycle
        cpu.fetch();
        cpu.decode_execute(DISASSEMBLE);

        // Handle screen updates
        window.draw(cpu.canDraw(), cpu.getFrameBuffer());

        // play sound
        audio_driver.beep(cpu.beep());

        // time it took to handle requests
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }


    return 0;
}
