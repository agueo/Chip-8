#include "../include/cpu.hpp"

void Cpu::op0(uint16_t _op){
    /*
     * op is 2 bytes 0xXX XX 1 byte == 0x00 8 buts
     * _op = 0x0XXX
     */
    uint8_t hi, lo;
    hi = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);

    if(lo == 0xE0) {
        printf("DISP CLS\n");
        // clear screen pixels
        // set draw flag
        // inc PC
        incrementPC();
    } else if (lo == 0xEE) {
        printf("RET\n");
        PC = stack[SP--];
    } else {
        printf("*CALL 0x%X\n", hi);
        incrementPC();
    }
}

void Cpu::op1(uint16_t _op){
    printf("JMP 0x%02X\n", _op);
    PC = _op;
}

void Cpu::op2(uint16_t _op){
    printf("Call 0x%02X\n", _op);
    stack[++SP] = PC;
    PC = _op;
}

void Cpu::op3(uint16_t _op){
    uint8_t hi, lo;
    hi = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    printf("SE V%X 0x%02X\n", hi, lo);
    if(v[hi] == lo) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op4(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    printf("SNE V%X 0x%X\n", x, lo);
    if(v[x] != lo) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op5(uint16_t _op){
    uint8_t x, y;
    x = (_op & 0xF00) >> 8;
    y = (_op & 0x0F0) >> 4;
    printf("SE V%X V%X\n", x, y);
    if(v[x] == v[y]) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::op6(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    printf("MVI V%X 0x%02X\n", x, lo);
    v[x] = lo;
    incrementPC();
}

void Cpu::op7(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    printf("ADD V%X 0x%X\n", x, lo);
    v[x] += lo;
    incrementPC();
}

void Cpu::op8(uint16_t _op){
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
            printf("LD V%X V%X\n", x, y);
            v[x] = v[y];
            break;
        case 0x1:
            printf("OR V%X V%X\n", x, y);
            v[x] = v[x] | v[y];
            break;
        case 0x2:
            printf("AND V%X V%X\n", x, y);
            v[x] = v[x] & v[y];
            break;
        case 0x3:
            printf("XOR V%X V%X\n", x, y);
            v[x] = v[x] ^ v[y];
            break;
        case 0x4:
            printf("ADD* V%X V%X\n", x, y);
            v[0xF] = (v[x] + v[y] > 0xFF) ? 1 : 0;
            v[x] = v[x] + v[y];
            break;
        case 0x5:
            printf("SUB* V%X V%X\n", x, y);
            // TODO - borrow flag
            v[x] = v[x] - v[y];
            break;
        case 0x6:
            printf("SHR* V%X\n", x);
            v[0xf] = (v[x] & 0x1);
            v[x] >>= 1;
            break;
        case 0x7:
            printf("SUB* V%X V%X\n", y, x);
            // TODO - Borrow flag
            v[x] = v[y] - v[x];
            break;
        case 0xE:
            printf("SHL* V%X\n", x);
            v[0xf] = (v[x] & 0x8) >> 7;
            v[x] <<= 1;
            break;
        default:
            printf("0x%x unimplemented", *op);
    }
    incrementPC();
}

void Cpu::op9(uint16_t _op){
    uint8_t x, y;
    x = (_op & 0xF00) >> 8;
    y = (_op & 0x0F0) >> 4;
    printf("SNE V%X V%X\n", x, y);
    if(v[x] == v[y]) {
        incrementPC();
    }
    incrementPC();
}

void Cpu::opA(uint16_t _op){
    printf("LD I [0x%03X]\n", _op);
    I = _op;
    incrementPC();
}

void Cpu::opB(uint16_t _op){
    printf("JMP V0 0x%03X\n", _op);
    PC = v[0] + _op;
}

void Cpu::opC(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    printf("RAND 0x%02X\n", lo);
    v[x] = (rand() % 256) & lo;
    incrementPC();
}

void Cpu::opD(uint16_t _op){
    printf("DRAW 0x%03X\n", _op);
    incrementPC();
}

void Cpu::opE(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(lo == 0x9E) {
        printf("KEY DOWN V%X\n", x);
        // TODO - get key down
        incrementPC();
    }
    else if(lo == 0xA1) {
        printf("KEY up V%X\n", x);
        // TODO - get key up
        incrementPC();
    }
    else {
        printf("opE 0x%03X not implemented\n", _op);
        exit(EXIT_FAILURE);
    }
    incrementPC();
}

void Cpu::opF(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);

    switch(lo) {
        case 0x07: // Timer get
            printf("TIMER GET\n");
            v[x] = delay;
            break;
        case 0x0A:
            printf("KEY WAIT\n");
            break;
        case 0x15:
            printf("TIMER SET\n");
            delay = v[x];
            break;
        case 0x18:
            printf("SOUND SET\n");
            sound = v[x];
            break;
        case 0x1E:
            printf("ADD I V%X\n", x);
            I += (uint16_t) v[x];
            break;
        case 0x29:
            printf("SPRITE\n");
            I = (uint16_t) v[x];
            break;
        case 0x33:
            printf("BCD\n");
            break;
        case 0x55:
            printf("REG DUMP\n");
            for(int i = 0; i < 16; i++){
                memory[I+i] = v[i];
            }
            break;
        case 0x65:
            printf("REG LOAD\n");
            for(int i = 0; i < 16; i++){
                v[i] = memory[I+i];
            }
            break;
        default:
            printf("OpF not yet implemented 0x%X\n", _op);
            exit(EXIT_FAILURE);
            break;
    }
    incrementPC();
}
