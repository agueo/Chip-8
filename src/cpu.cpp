#include "../include/cpu.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Cpu::Cpu(){ init_cpu(); }
Cpu::~Cpu(){}

void Cpu::init_cpu(){
    std::cout << "Initializing CPU..." << std::endl;

    // initialize memory
    memset(v, 0, sizeof(v));
    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    memset(key_state, 0, sizeof(key_state));
    memset(saved_key_state, 0, sizeof(saved_key_state));
    memset(frame_buffer, WHITE_PIXEL, sizeof(frame_buffer));

    // initialize pointers and flags
    I = 0;
    delay = 0;
    sound = 0;
    SP = 0;
    PC = 0x200;
    draw_flag = 0;
    halt_flag = 0;
    wait_for_key = 0;
    halt_cpu = false;
    op = NULL;
    // load font data into memory
    memcpy(&(memory[FONT_BASE]), font_data, sizeof(font_data));

    std::cout << "Initialization complete!" << std::endl;
}

/* TODO - optimize RC
 * FIXME - BUG when reading in some files
 * Look into reading files using C++ iostream/fstream
 */
bool Cpu::loadRom(const char *filename) 
{
    std::cout << "Loading " << filename << std::endl;
    FILE *fp = nullptr;
    fp = fopen(filename, "rb");
    if(fp == NULL) {
        std::cout << "Error reading file " << filename << std::endl;
        return 0;
    }

    // get the filesize 
    fseek(fp, 0l, SEEK_END);
    uint file_size = ftell(fp);
    rewind(fp);

    std::cout << "Filesize: " << file_size << std::endl;

    if(file_size > MAX_ROM_SIZE) {
        std::cout << "Filesize too big!" << std::endl;
        return 0;
    }

    std::cout << "Creating buffer for file contents" << std::endl;
    uint8_t *buffer = (uint8_t *)calloc(1, sizeof(uint8_t) * file_size);

    std::cout << "Reading in contents" << std::endl;
    size_t bytes = fread(buffer, sizeof(uint8_t), file_size, fp);

    std::cout << "Closing filehandle" << std::endl;
    fclose(fp);

    std::cout << "Closed filehandle" << std::endl;
    if(bytes != file_size) {
        std::cout << "Failed to read FILE!";
        free(buffer);
        return 0;
    }

    std::cout << "Trying to copy ROM to memory" << std::endl;
    // copy the ROM data to memory
    for(uint i = 0; i < file_size; ++i) {
        memory[0x200 + i] = buffer[i];
    }
    std::cout << "ROM Copied successfully!\n" << std::endl;

    free(buffer);

    return 1;
}

/*
 * disassemble program
 * printout PC and opcode at memory[PC]
 */
void Cpu::disassemble_program() {
    // get the op code
    op = &memory[PC];
    // print out the contents of the full program
    printf("PC: 0x%04X OP: 0x%02X %02X\n", PC, op[0], op[1]);
    PC++;
}

/*
 * Fetches a opcode from memory and sets
 * op pointer to it
 * op -> &memory[pc]
 * op[0] contains high byte
 * op[1] contains low byte
 */
void Cpu::fetch(){
    // get op code from memory
    op = &memory[PC];
}

void Cpu::decode_execute(){
    // opcode - 0xXX XX
    // 2 bytes each

    // parse opcode into op and args
    uint8_t op_code = (*op & 0xF0) >> 4; 
    uint16_t _op = ((op[0] & 0x0F) << 8) + op[1];

    printf("PC: 0x%04X OP: 0x%02X %02X : ", PC, op[0], op[1]);
    // pass to correct function
    switch(op_code) {
        case 0x0:
            op0(_op);
            break;
        case 0x1:
            op1(_op);
            break;
        case 0x2:
            op2(_op);
            break;
        case 0x3:
            op3(_op);
            break;
        case 0x4:
            op4(_op);
            break;
        case 0x5:
            op5(_op);
            break;
        case 0x6:
            op6(_op);
            break;
        case 0x7:
            op7(_op);
            break;
        case 0x8:
            op8(_op);
            break;
        case 0x9:
            op9(_op);
            break;
        case 0xA:
            opA(_op);
            break;
        case 0xB:
            opB(_op);
            break;
        case 0xC:
            opC(_op);
            break;
        case 0xD:
            opD(_op);
            break;
        case 0xE:
            opE(_op);
            break;
        case 0xF:
            opF(_op);
            break;
        default:
            std::cout << "unimplemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
}

void Cpu::reset_system() {
    op = nullptr;
    // Reset PC
    PC = 0x200;
    // Clear Stack
    memset(stack, 0, sizeof(stack));
    // Clear display
    memset(frame_buffer, BLACK_PIXEL, sizeof(frame_buffer));
    // Reset register values
    memset(v, 0, sizeof(v));
    // Reset keyboard
    memset(key_state, 0, sizeof(key_state));
    memset(saved_key_state, 0, sizeof(saved_key_state));
}

void Cpu::decrementTimers() {
    sound--;
    delay--;
}