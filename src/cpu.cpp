#include <algorithm>
#include <array>
#include <fstream>
#include <vector>
#include "cpu.hpp"

Cpu::Cpu() 
: op{nullptr}, delay{0}, sound{0}, SP{0}, I{0}, PC{0x200}, v{}, stack{}, 
saved_key_state{}, draw_flag{false}, halt_flag{false}, wait_for_key{false},
halt_cpu{false}, memory(4096,0), frame_buffer(64*32, 0), key_state{}
{
    // Copy font into the memory
    std::copy(font_data.cbegin(), font_data.cend(), memory.begin());
}

bool Cpu::loadRom(const char *filename) noexcept {
    std::cout << "Loading " << filename << std::endl;

    std::ifstream rom (filename, std::ios::in | std::ios::binary);
    if(!rom.is_open()) {
        std::cout << "Failed to open file: " << filename << '\n';
        return false;
    }

    // Copy the file contents to a vector
    std::vector<uint8_t> rom_data ((std::istreambuf_iterator<char>(rom)), (std::istreambuf_iterator<char>()));
    if (rom_data.size() > MAX_ROM_SIZE){
        std::cout << "Filesize too big!\n";
        return false;
    }

    std::cout << "Trying to copy ROM to memory" << std::endl;
    // copy the ROM data to memory
    std::copy_n(rom_data.cbegin(), rom_data.size(), memory.begin()+0x200);
    std::cout << "ROM Copied successfully!\n" << std::endl;

    return true;
}

/*
 * Fetches a opcode from memory and sets
 * op pointer to it
 * op -> &memory[pc]
 * op[0] contains high byte
 * op[1] contains low byte
 */
void Cpu::fetch() noexcept {
    // get op code from memory
    op = &memory[PC];
}

void Cpu::decode_execute(bool disassemble) noexcept {
    draw_flag = false;
    // opcode - 0xXX XX
    // 2 bytes each

    // parse opcode into op and args
    uint8_t op_code = (*op & 0xF0) >> 4; 
    uint16_t _op = ((op[0] & 0x0F) << 8) + op[1];

    if(disassemble) printf("PC: 0x%04X OP: 0x%02X %02X : ", PC, op[0], op[1]);
    // pass to correct function
    // we can make this a LUT
    switch(op_code) {
        case 0x0:
            op0(_op, disassemble);
            break;
        case 0x1:
            op1(_op, disassemble);
            break;
        case 0x2:
            op2(_op, disassemble);
            break;
        case 0x3:
            op3(_op, disassemble);
            break;
        case 0x4:
            op4(_op, disassemble);
            break;
        case 0x5:
            op5(_op, disassemble);
            break;
        case 0x6:
            op6(_op, disassemble);
            break;
        case 0x7:
            op7(_op, disassemble);
            break;
        case 0x8:
            op8(_op, disassemble);
            break;
        case 0x9:
            op9(_op, disassemble);
            break;
        case 0xA:
            opA(_op, disassemble);
            break;
        case 0xB:
            opB(_op, disassemble);
            break;
        case 0xC:
            opC(_op, disassemble);
            break;
        case 0xD:
            opD(_op, disassemble);
            break;
        case 0xE:
            opE(_op, disassemble);
            break;
        case 0xF:
            opF(_op, disassemble);
            break;
        default:
            std::cout << "unimplemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
}

void Cpu::reset_system() noexcept {
    op = nullptr;
    // Reset PC
    PC = 0x200;
    // Clear Stack
    std::fill(stack.begin(), stack.end(), 0);
    // Clear display
    std::fill(frame_buffer.begin(), frame_buffer.end(), BLACK_PIXEL);
    // Reset register values
    std::fill(v.begin(), v.end(), 0);
    // Reset keyboard
    std::fill(key_state.begin(), key_state.end(), 0);
    std::fill(saved_key_state.begin(), saved_key_state.end(), 0);
}

void Cpu::decrementTimers() noexcept {
    // don't wrap around
    if (sound > 0) --sound;
    if (delay > 0) --delay;
}