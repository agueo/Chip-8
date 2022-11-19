#ifndef _CPU_H_
#define _CPU_H_

#include <array>
#include <iostream>
#include <vector>
#include "constants.hpp"

#define DISASSEMBLE (0)

class Cpu {
public:
    Cpu();
    ~Cpu() noexcept = default;

    /* core functions */
    bool loadRom(const char *filename) noexcept;
    void fetch() noexcept;
    void decode_execute(bool disasemble = false) noexcept;
    void reset_system() noexcept;

    /* Helper functions */
    void setHaltCpu(bool halt) noexcept { halt_cpu = halt; }
    bool isHalted() const noexcept { return halt_cpu; }

    void setDrawFlag(bool draw) noexcept { draw_flag = draw; }
    bool canDraw() const noexcept { return draw_flag; }
    bool beep() const noexcept { return sound > 0; }
    uint32_t *getFrameBuffer() noexcept { return frame_buffer.data(); }

    void incrementPC() noexcept { PC += 2; }
    void decrementTimers() noexcept;

    // Opcode Functions
    void op0(uint16_t _op, bool disassemble=false) noexcept; void op1(uint16_t _op, bool disassemble=false) noexcept; 
    void op2(uint16_t _op, bool disassemble=false) noexcept; void op3(uint16_t _op, bool disassemble=false) noexcept;
    void op4(uint16_t _op, bool disassemble=false) noexcept; void op5(uint16_t _op, bool disassemble=false) noexcept; 
    void op6(uint16_t _op, bool disassemble=false) noexcept; void op7(uint16_t _op, bool disassemble=false) noexcept;
    void op8(uint16_t _op, bool disassemble=false) noexcept; void op9(uint16_t _op, bool disassemble=false) noexcept; 
    void opA(uint16_t _op, bool disassemble=false) noexcept; void opB(uint16_t _op, bool disassemble=false) noexcept;
    void opC(uint16_t _op, bool disassemble=false) noexcept; void opD(uint16_t _op, bool disassemble=false) noexcept; 
    void opE(uint16_t _op, bool disassemble=false) noexcept; void opF(uint16_t _op, bool disassemble=false) noexcept;

private:
    uint8_t *op{nullptr};               // helper variable to hold opcode
    uint8_t  delay{};                   // delay timer
    uint8_t  sound{};                   // sound timer
    uint8_t  SP{};                      // stack pointer
    uint16_t I{};                       // direct memory register
    uint16_t PC{0x200};                 // Program counter
    std::array<uint8_t, 16>  v{};       // Registers

    std::array<uint16_t, 16> stack{};   // stack 64 bytes
    std::array<uint8_t, 16> saved_key_state{}; // previous key state
    
    bool draw_flag{};                   // flag for whether we should draw
    bool halt_flag{};                   // flag for quiting game
    bool wait_for_key{};                // flag for whether we need to wait for a key press
    bool halt_cpu{};

    std::vector<uint8_t>  memory{};     // 4K RAM - ROM starts at 0x200
    std::vector<uint32_t> frame_buffer; // Screen memory

public:
    std::array<uint8_t, 16> key_state;  // Key map state
};

// font data
static const std::vector<uint8_t> font_data= {
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
