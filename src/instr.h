#pragma once
#include <array>

enum opcode {
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, LDC, ST, 
    BLT, BNZ, B, 
    J, JLT, JNZ,
    CMP, 
    HALT
};

enum addrMode {
    REGISTER,
    IMMEDIATE,
};


struct Instr {
    opcode opcode;
    std::array<std::pair<unsigned char, addrMode>, 3> operands;
};