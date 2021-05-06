#pragma once
#include <array>
#include <stdint.h>

enum class opcode {
    NOP,
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, LDC, ST, STC,
    BLT, BNZ, B, 
    J, JLT, JNZ,
    CMP, 
    HALT
};

const int RS_COUNT = 4;

const int EXEC_COUNT = 4;

typedef int RSID;

enum class RSType {ALU, LDST, BRANCH};

typedef int ROBID;

enum class InstrType { REG, MEM, BRANCH};

struct RSEntry {
    opcode opcode;
    RSID RSd;
    RSID RSn;
    RSID RSi;
    int Rd;
    int Rn;
    int Ri;
    bool busy;
    bool ready;
    ROBID ROBId;
};


struct ROBEntry {
    ROBID id;
    InstrType type;
    int result;
    int dest;
    bool ready;
    //bool inUse;
};

enum class addrMode {
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
    int npc;
    int RSID;
};

// struct FetchedInstr : Instr {
//     int npc;
// };


// struct IssuedInstr : FetchedInstr {
//     int RSID;
// };

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
        cir = {opcode::NOP, 0, 0, 0, false};
        ALUOut = 0;
        MEMLoadData = 0;
        cond = false;
    }
};