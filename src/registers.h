#include "instr.h"

struct IFID {
    Instr cir;
    int npc;
};

struct IDEX {
    int Rd;
    int Rn;
    int Ri;
    int npc;
    Instr cir;
};

struct PipelineRegister {
    int Rd;
    int Rn;
    int Ri;
    int npc;
    Instr cir;
    int ALUOut;
    int MEMLoadData;
    bool cond;
};

// struct EXMEM {
//     Instr cir;
//     int out;
//     int Rd;
//     int Rn;
//     int Ri;
// };

// struct MEMWB {

// }