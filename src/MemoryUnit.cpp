#include "MemoryUnit.h"

MemoryUnit::MemoryUnit(int *MEM, PipelineRegister *exmem, PipelineRegister *memwb) {
    MemoryUnit::MEM = MEM;
    MemoryUnit::exmem = exmem;
    MemoryUnit::memwb = memwb;
}

int MemoryUnit::memory() {
    int tick = 0;
    *(memwb) = *(exmem);
    opcode opcode = exmem->cir.opcode;
    //Load
    if(opcode >= LD && opcode <= LDC) {
        memwb->MEMLoadData = MEM[exmem->ALUOut];
        tick++;
    }
    //Store
    else if(opcode == ST) {
        MEM[exmem->ALUOut] = exmem->Rd;
        tick++;
    }
    else if(opcode == STC) {
        MEM[exmem->ALUOut] = exmem->Ri;
        tick++;
    }

    exmem->active = false;
    memwb->active = true;

    return 0;
}