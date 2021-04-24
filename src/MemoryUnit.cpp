#include "MemoryUnit.h"
#include <iostream>

MemoryUnit::MemoryUnit(int *pc, int *MEM, PipelineRegister *exmem, PipelineRegister *memwb) {
    MemoryUnit::MEM = MEM;
    MemoryUnit::exmem = exmem;
    MemoryUnit::memwb = memwb;
    MemoryUnit::pc = pc;
}

int MemoryUnit::memory() {
    int tick = 0;
    *(memwb) = *(exmem);
    memwb->active = false;

    opcode opcode = exmem->cir.opcode;
    //Load
    if(opcode == LD) {
        memwb->MEMLoadData = MEM[exmem->ALUOut];
        tick++;
    }
    else if(opcode == LDC) {
        memwb->MEMLoadData = exmem->Ri;

    }
    //Store
    else if(opcode == ST) {
        MEM[exmem->ALUOut] = exmem->Rd;
        tick++;
    }
    else if(opcode == STC) {
        MEM[exmem->ALUOut] = exmem->Rd;
        tick++;
    }
    else if((exmem->cir.opcode >= BLT) && (exmem->cir.opcode <= JNZ) && exmem->cond) {
            // ifid->cir = INSTR[exmem->ALUOut];
            // ifid->npc = exmem->ALUOut; //FETCH CORRECT INSTR HERE
            (*pc) = exmem->ALUOut;
        }

    exmem->active = false;
    memwb->active = true;

    return 0;
}