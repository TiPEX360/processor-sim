#pragma once
#include <array>

enum opcode {
    NOP,
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, LDC, ST, STC,
    BLT, BNZ, B, 
    J, JLT, JNZ,
    CMP, 
    HALT
};


class PipelineRegister {
public:
    bool active;
    int Rd;
    int Rn;
    int Ri;
    int npc;
    Instr cir;
    int ALUOut;
    int MEMLoadData;
    bool cond;
    
    PipelineRegister() {
        active = false;
        Rd = 0;
        Rn = 0;
        Ri = 0;
        npc = 0;
        cir = {NOP, 0, 0, 0, false};
        ALUOut = 0;
        MEMLoadData = 0;
        cond = false;
    }
};

typedef int RSID;

enum addrMode {
    REGISTER,
    IMMEDIATE,
};

struct Register {
    int32_t value;
    int RS;
};

struct Instr {
    opcode opcode;
    unsigned char Rd;
    unsigned char Rn;
    signed int Ri : 10; 
    bool immediate;
};