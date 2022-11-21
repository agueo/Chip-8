#include <algorithm>
#include <array>
#include <vector>

#include "cpu.hpp"

void Cpu::op0(uint16_t _op, bool disassemble) noexcept {
    /*
     * op is 2 bytes 0xXX XX 1 byte == 0x00 8 buts
     * _op = 0x0XXX
     */
    uint8_t lo;
    lo = (_op & 0x0FF);

    if(lo == 0xE0) {
        if(disassemble) printf("DISP CLS\n");
        // clear screen pixels
        std::fill(frame_buffer.begin(), frame_buffer.end(), BLACK_PIXEL);
        // set draw flag
        setDrawFlag(true);
        // inc PC
        incrementPC();
    } else if (lo == 0xEE) {
        if(disassemble) printf("RET\n");
        PC = stack[SP--];
        incrementPC();
    } else {
        if(disassemble) printf("*CALL 0x%X\n", _op);
        //PC = _op;
        incrementPC();
    }
}

void Cpu::op1(uint16_t _op, bool disassemble) noexcept {
    if(disassemble) printf("JMP 0x%02X\n", _op);
    PC = _op;
}

void Cpu::op2(uint16_t _op, bool disassemble) noexcept {
    if(disassemble) printf("Call 0x%02X\n", _op);
    stack[++SP] = PC;
    PC = _op;
}

void Cpu::op3(uint16_t _op, bool disassemble) noexcept {
    uint8_t hi, lo;
    hi = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(disassemble) printf("SE V%X 0x%02X\n", hi, lo);
    if(v[hi] == lo) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op4(uint16_t _op, bool disassemble) noexcept {
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(disassemble) printf("SNE V%X 0x%X\n", x, lo);
    if(v[x] != lo) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op5(uint16_t _op, bool disassemble) noexcept {
    uint8_t x, y;
    x = (_op & 0xF00) >> 8;
    y = (_op & 0x0F0) >> 4;
    if(disassemble) printf("SE V%X V%X\n", x, y);
    if(v[x] == v[y]) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op6(uint16_t _op, bool disassemble) noexcept {
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(disassemble) printf("MVI V%X 0x%02X\n", x, lo);
    v[x] = lo;
    incrementPC();
}

void Cpu::op7(uint16_t _op, bool disassemble) noexcept{
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(disassemble) printf("ADD V%X 0x%X\n", x, lo);
    v[x] += lo;
    incrementPC();
}

void Cpu::op8(uint16_t _op, bool disassemble) noexcept{
    /*
     * _op is 2 bytes long
     * _op = 0xFFFF
     * 
     */
    uint8_t x, y, type;
    x = (_op & 0xF00) >> 8;
    y = (_op & 0x0F0) >> 4;
    type = (_op & 0x00F);

    switch(type) {
        case 0x0:
            if(disassemble) printf("LD V%X V%X\n", x, y);
            v[x] = v[y];
            break;
        case 0x1:
            if(disassemble) printf("OR V%X V%X\n", x, y);
            v[x] = v[x] | v[y];
            break;
        case 0x2:
            if(disassemble) printf("AND V%X V%X\n", x, y);
            v[x] = v[x] & v[y];
            break;
        case 0x3:
            if(disassemble) printf("XOR V%X V%X\n", x, y);
            v[x] = v[x] ^ v[y];
            break;
        case 0x4:
            if(disassemble) printf("ADD* V%X V%X\n", x, y);
            v[0xF] = (v[x] > ( 0xFF - v[y] )) ? 1 : 0;
            v[x] = v[x] + v[y];
            break;
        case 0x5:
            if(disassemble) printf("SUB* V%X V%X\n", x, y);
            // borrow flag - if borrow necessary -> 0 else 1
            v[0xf] = (v[x] > v[y]) ? 1 : 0;
            v[x] = v[x] - v[y];
            break;
        case 0x6:
            if(disassemble) printf("SHR* V%X\n", x);
            v[0xf] = (v[x] & 0x1);
            v[x] >>= 1;
            break;
        case 0x7:
            if(disassemble) printf("SUB* V%X V%X\n", y, x);
            // borrow flag - if borrow necessary -> 0 else 1
            v[0xf] = (v[y] > v[x]) ? 1 : 0;
            v[x] = v[y] - v[x];
            break;
        case 0xE:
            if(disassemble) printf("SHL* V%X\n", x);
            v[0xf] = (v[x] & 0x8) >> 7;
            v[x] <<= 1;
            break;
        default:
            if(disassemble) printf("0x%x unimplemented", *op);
    }
    incrementPC();
}

void Cpu::op9(uint16_t _op, bool disassemble) noexcept{
    uint8_t x, y;
    x = (_op & 0xF00) >> 8;
    y = (_op & 0x0F0) >> 4;
    if(disassemble) printf("SNE V%X V%X\n", x, y);
    if(v[x] != v[y]) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::opA(uint16_t _op, bool disassemble) noexcept{
    if(disassemble) printf("LD I [0x%03X]\n", _op);
    I = _op;
    incrementPC();
}

void Cpu::opB(uint16_t _op, bool disassemble) noexcept{
    if(disassemble) printf("JMP V0 0x%03X\n", _op);
    PC = v[0] + _op;
}

void Cpu::opC(uint16_t _op, bool disassemble) noexcept{
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(disassemble) printf("RAND 0x%02X\n", lo);
    v[x] = (rand() % 256) & lo;
    incrementPC();
}

void Cpu::opD(uint16_t _op, bool disassemble) noexcept{
    uint8_t x, y, height;
    x = (_op  & 0xF00) >> 8;
    y = (_op  & 0x0F0) >> 4;
    height = (_op  & 0x00F);
    if(disassemble) printf("DRAW 0x%03X\n", _op);

    v[0xF] = 0;
    for(int line = 0; line < height; ++line) {
        for(int bit = 0; bit < 8; ++bit) {
            uint32_t sprite = memory[I + line];
            // get the pixel to set from MSB
            // if we need to set the pixel 
            if(sprite & (0x80 >> bit)) {
                uint32_t index = (((v[x] + bit) % WIDTH) + ((v[y] + line) % HEIGHT) * WIDTH);
                if(frame_buffer[index] == WHITE_PIXEL) {
                    v[0xf] = 1;
                }
                frame_buffer[index] ^= WHITE_PIXEL;
            }
        }
    }

    setDrawFlag(true);
    incrementPC();
}

void Cpu::opE(uint16_t _op, bool disassemble) noexcept{
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(lo == 0x9E) {
        if(disassemble) printf("KEY DOWN V%X %x\n", x, v[x]);
        // if key pressed
        if(key_state[v[x]] == 1) {
            incrementPC();
        }
    }
    else if(lo == 0xA1) {
        if(disassemble) printf("KEY up V%X %x\n", x, v[x]);
        // if key not pressed
        if(key_state[v[x]] == 0) {
            incrementPC();
        }
    }
    else {
        if(disassemble) printf("opE 0x%03X not implemented\n", _op);
        exit(EXIT_FAILURE);
    }
    incrementPC();
}

void Cpu::opF(uint16_t _op, bool disassemble) noexcept {
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);

    switch(lo) {
        case 0x07: // Timer get
            if(disassemble) printf("TIMER GET\n");
            v[x] = delay;
            break;
        case 0x0A:
            if(disassemble) printf("KEY WAIT\n");
            if(wait_for_key == 0) {
                // start waitkey cycle
                std::copy(key_state.cbegin(), key_state.cend(), saved_key_state.begin());
                // set waitkey
                wait_for_key = true;
                // don't increment PC
                return;
            } else {
                // look for change in key state
                for(int i = 0; i < 0xF; ++i) {
                    // if key state changed
                    if((saved_key_state[i] == 0) && (key_state[i] == 1)) {
                        // reset waitkey flag
                        wait_for_key = false;
                        // save key changed
                        v[x] = i;
                        // increment
                        incrementPC();
                        return;
                    }
                    // otherwise save key state
                    saved_key_state[i] = key_state[i];
                }
                // if no key changed
                return;
            }
            break;
        case 0x15:
            if(disassemble) printf("TIMER SET V%X\n", x);
            delay = v[x];
            break;
        case 0x18:
            if(disassemble) printf("SOUND SET V%X\n", x);
            sound = v[x];
            break;
        case 0x1E:
            if(disassemble) printf("ADD I V%X\n", x);
            I += (uint16_t) v[x];
            break;
        case 0x29:
            if(disassemble) printf("SPRITE V%X\n", x);
            I = (uint16_t) (v[x] * 5);
            break;
        case 0x33:
            if(disassemble) printf("BCD 0x%02X\n",v[x]);
            // ones place
            memory[I + 2] = (v[x] % 100) % 10;
            // tens place
            memory[I + 1] = (v[x] / 10) % 10;
            // hundreds place
            memory[I] = v[x] / 100;
            break;
        case 0x55:
            if(disassemble) printf("REG DUMP V%X\n", x);
            for(int i = 0; i <= x; i++){
                memory[I+i] = v[i];
            }
            I += x + 1;
            break;
        case 0x65:
            if(disassemble) printf("REG LOAD V%X\n", x);
            for(int i = 0; i <= x; i++){
                v[i] = memory[I+i];
            }
            I += x + 1;
            break;
        default:
            if(disassemble) printf("OpF not yet implemented 0x%X\n", _op);
            exit(EXIT_FAILURE);
            break;
    }
    incrementPC();
}
