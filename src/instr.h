#pragma once
#include <array>
#include <stdint.h>

enum class opcode {
    NOP,
    ADD,  MUL, SUB, DIV,
    LSH, RSH, AND, OR, XOR,
    LD, LDC, ST, STC,
    BLT, BZ, BNZ, B, 
    J, JZ, JLT, JNZ,
    HALT
};

extern int SCALE_WIDTH;


extern int RS_COUNT;

extern int EXEC_COUNT;

extern int instrCount;

typedef int RSID;

enum class RSType {ALU, LDST, BRANCH};

typedef int ROBID;

enum class InstrType { REG, MEM, BRANCH, NOP, HALT};

struct RSEntry {
    opcode opcode;
    RSID RSd;
    RSID RSn;
    RSID RSi;
    int Rd;
    int Rn;
    int Ri;
    bool executing;
    ROBID ROBId;
    uint32_t bpc;
};


struct ROBEntry {
    ROBID id;
    InstrType type;
    int result;
    int dest;
    bool ready;
    uint32_t bpc;
};

enum class addrMode {
    REGISTER,
    IMMEDIATE,
};

enum class BPType {
    STATIC,
    DYNAMIC,
    FIXEDTAKE,
    FIXEDSKIP
};
extern BPType PREDICTIONTYPE;


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
    int branchTaken;
    int RSID;
    uint32_t bpc;
};

struct BundleWriteLog {
    int dest;
    ROBID ROBId;
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
        cir = {opcode::NOP, 0, 0, 0, false};
        ALUOut = 0;
        MEMLoadData = 0;
        cond = false;
    }
};