#include <iostream>
#include <fstream>

enum opcode {
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, ST, 
    BLT, BNZ, B, 
    J, JLT, JNZ,
    CMP, 
    HALT
};

enum addrMode {
    REGISTER,
    IMMEDIATE,
};

typedef struct instr {
    opcode opcode;
    std::pair<signed char, addrMode> a1;
    std::pair<signed char, addrMode> a2;
    std::pair<signed char, addrMode> a3;
};

int main(int argc, char *argv[]) {
    bool halt = false;
    int cycles = 0;
    
    int32_t RF[32];
    int32_t MEM[1024];
    instr INSTR[512];

    while(!halt) {

    }

    return 0;
}