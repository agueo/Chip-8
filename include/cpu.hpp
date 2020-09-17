#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include <iostream>

#define MAX_ROM_SIZE (4096 - 0x200)
#define FONT_BASE (0)
#define FONT_SIZE (5*16)
#define DEBUG (1)

class Cpu 
{
    private:
    // opcode variable ?
    uint8_t *op;

    uint8_t  v[16];         // Registers
    uint16_t I;             // direct memory register
    uint8_t  delay;         // delay timer
    uint8_t  sound;         // sound timer
    uint8_t  SP;            // stack pointer
    uint16_t PC;            // Program counter

    uint16_t stack[16];     // stack 64 bytes
    uint8_t key_state[16];  // Key map state
    uint8_t saved_key_state[16]; // previous key state
    uint8_t draw_flag;      // flag for whether we should draw
    uint8_t halt_flag;      // flag for quiting game
    uint8_t wait_for_key;   // flag for whether we need to wait for a key press
    bool halt_cpu;

    uint8_t  memory[4096];  // 4K RAM - ROM starts at 0x200
    uint32_t frame_buffer[64*32]; // Screen memory

    public:
    Cpu();
    ~Cpu();
    /* core functions */
    void init_cpu();
    bool loadRom(const char *filename);
    void fetch();
    void decode_execute();
    void reset_system();

    /* Helper functions */
    bool isHalted() { return halt_cpu; }
    void setHaltCpu() { halt_cpu = true; }
    uint16_t debugPC() { return PC; }
    void incrementPC() { PC += 2; }

    void disassemble_program();

    // Opcode Functions
    void op0(uint16_t _op); void op1(uint16_t _op); void op2(uint16_t _op); void op3(uint16_t _op);
    void op4(uint16_t _op); void op5(uint16_t _op); void op6(uint16_t _op); void op7(uint16_t _op);
    void op8(uint16_t _op); void op9(uint16_t _op); void opA(uint16_t _op); void opB(uint16_t _op);
    void opC(uint16_t _op); void opD(uint16_t _op); void opE(uint16_t _op); void opF(uint16_t _op);
};

// font data
static const uint8_t font_data[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0x70, // 2
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