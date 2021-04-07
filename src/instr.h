#pragma once
#include <array>

enum opcode {
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, LDC, ST, STC,
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
    unsigned char Rd;
    unsigned char Rn;
    signed int Ri : 10; 
    bool immediate;
};