#pragma once
#include "instr.h"

// struct IFID {
//     Instr cir;
//     int npc;
// };

// struct IDEX {
//     int Rd;
//     int Rn;
//     int Ri;
//     int npc;
//     Instr cir;
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
        cir = {NOP, 0, 0, 0, false};
        ALUOut = 0;
        MEMLoadData = 0;
        cond = false;
    }
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