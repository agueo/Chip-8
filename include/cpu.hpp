#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.hpp"

#define DISASSEMBLE (0)

class Cpu 
{
    private:
    uint8_t *op;

    uint8_t  v[16];         // Registers
    uint8_t  delay;         // delay timer
    uint8_t  sound;         // sound timer
    uint8_t  SP;            // stack pointer
    uint16_t I;             // direct memory register
    uint16_t PC;            // Program counter

    uint16_t stack[16];     // stack 64 bytes
    uint8_t saved_key_state[16]; // previous key state
    
    bool draw_flag;      // flag for whether we should draw
    bool halt_flag;      // flag for quiting game
    bool wait_for_key;   // flag for whether we need to wait for a key press
    bool halt_cpu;

    uint8_t  memory[4096];  // 4K RAM - ROM starts at 0x200

    public:
    uint32_t frame_buffer[64*32]; // Screen memory
    uint8_t key_state[16];  // Key map state

    private: 
    void init_cpu();

    public:
    Cpu();
    ~Cpu();
    /* core functions */
    bool loadRom(const char *filename);
    void fetch();
    void decode_execute(bool disasemble = false);
    void reset_system();

    /* Helper functions */
    void setHaltCpu(bool halt) { halt_cpu = halt; }
    bool isHalted() { return halt_cpu; }

    void setDrawFlag(bool draw) { draw_flag = draw; }
    bool canDraw() { return draw_flag; }
    uint32_t *getFrameBuffer() { return frame_buffer; }

    uint16_t debugPC() { return PC; }
    void incrementPC() { PC += 2; }

    void disassemble_program();

    void decrementTimers();

    // Opcode Functions
    void op0(uint16_t _op, bool disassemble=false); void op1(uint16_t _op, bool disassemble=false); void op2(uint16_t _op, bool disassemble=false); void op3(uint16_t _op, bool disassemble=false);
    void op4(uint16_t _op, bool disassemble=false); void op5(uint16_t _op, bool disassemble=false); void op6(uint16_t _op, bool disassemble=false); void op7(uint16_t _op, bool disassemble=false);
    void op8(uint16_t _op, bool disassemble=false); void op9(uint16_t _op, bool disassemble=false); void opA(uint16_t _op, bool disassemble=false); void opB(uint16_t _op, bool disassemble=false);
    void opC(uint16_t _op, bool disassemble=false); void opD(uint16_t _op, bool disassemble=false); void opE(uint16_t _op, bool disassemble=false); void opF(uint16_t _op, bool disassemble=false);
};

// font data
static const uint8_t font_data[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

#endif
