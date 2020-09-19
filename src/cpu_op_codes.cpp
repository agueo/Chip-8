#include "../include/cpu.hpp"

void Cpu::op0(uint16_t _op){
    /*
     * op is 2 bytes 0xXX XX 1 byte == 0x00 8 buts
     * _op = 0x0XXX
     */
    uint8_t lo;
    lo = (_op & 0x0FF);

    if(lo == 0xE0) {
        printf("DISP CLS\n");
        // clear screen pixels
        memset(frame_buffer, BLACK_PIXEL, sizeof(frame_buffer));
        // set draw flag
        setDrawFlag(true);
        // inc PC
        incrementPC();
    } else if (lo == 0xEE) {
        printf("RET\n");
        PC = stack[SP--];
        incrementPC();
    } else {
        printf("*CALL 0x%X\n", _op);
        //PC = _op;
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
            v[0xF] = (v[x] > ( 0xFF - v[y] )) ? 1 : 0;
            printf("- x %x y %x c %x\n", v[x], v[y], v[0xF]);
            v[x] = v[x] + v[y];
            break;
        case 0x5:
            printf("SUB* V%X V%X\n", x, y);
            // borrow flag - if borrow necessary -> 0 else 1
            v[0xf] = (v[y] > v[x]) ? 0 : 1;
            v[x] = v[x] - v[y];
            break;
        case 0x6:
            printf("SHR* V%X\n", x);
            v[0xf] = (v[x] & 0x1);
            v[x] >>= 1;
            break;
        case 0x7:
            printf("SUB* V%X V%X\n", y, x);
            // borrow flag - if borrow necessary -> 0 else 1
            v[0xf] = (v[x] > v[y]) ? 0 : 1;
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
    uint8_t x, y, height;
    x = (_op  & 0xF00) >> 8;
    y = (_op  & 0x0F0) >> 4;
    height = (_op  & 0x00F);
    printf("DRAW 0x%03X\n", _op);

    v[0xF] = 0;
    for(int line = 0; line < height; line++) {
        for(int bit = 0; bit < 8; bit++) {
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

void Cpu::opE(uint16_t _op){
    uint8_t x, lo;
    x = (_op & 0xF00) >> 8;
    lo = (_op & 0x0FF);
    if(lo == 0x9E) {
        printf("KEY DOWN V%X %x\n", x, v[x]);
        // if key pressed
        if(key_state[v[x]] == 1) {
            printf("%x was pressed\n", v[x]);
            incrementPC();
        }
    }
    else if(lo == 0xA1) {
        printf("KEY up V%X %x\n", x, v[x]);
        // if key not pressed
        if(key_state[v[x]] == 0) {
            printf("%x was up\n", v[x]);
            incrementPC();
        }
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
            printf(" - %d\n", delay);
            v[x] = delay;
            break;
        case 0x0A:
            printf("KEY WAIT\n");
            if(wait_for_key == 0) {
                // start waitkey cycle
                memcpy(saved_key_state, key_state, sizeof(key_state));
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
            printf("TIMER SET V%X\n", x);
            printf(" - %d\n", v[x]);
            delay = v[x];
            break;
        case 0x18:
            printf("SOUND SET V%X\n", x);
            printf(" - %d\n", v[x]);
            sound = v[x];
            break;
        case 0x1E:
            printf("ADD I V%X\n", x);
            I += (uint16_t) v[x];
            break;
        case 0x29:
            printf("SPRITE V%X\n", x);
            I = (uint16_t) v[x] * 5;
            break;
        case 0x33:
            printf("BCD 0x%02X\n",v[x]);
            // ones place
            memory[I + 2] = (v[x] % 100) % 10;
            // tens place
            memory[I + 1] = (v[x] / 10) % 10;
            // hundreds place
            memory[I] = v[x] / 100;
            printf(" - %d%d%d\n", memory[I], memory[I+1], memory[I+2]);
            break;
        case 0x55:
            printf("REG DUMP V%X\n", x);
            for(int i = 0; i <= x; i++){
                memory[I+i] = v[i];
            }
            I += x + 1;
            break;
        case 0x65:
            printf("REG LOAD V%X\n", x);
            for(int i = 0; i <= x; i++){
                printf(" - V%X - %d\n", i, memory[I+i]);
                v[i] = memory[I+i];
            }
            I += x + 1;
            break;
        default:
            printf("OpF not yet implemented 0x%X\n", _op);
            exit(EXIT_FAILURE);
            break;
    }
    incrementPC();
}
