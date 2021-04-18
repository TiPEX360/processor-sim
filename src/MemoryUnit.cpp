#include "MemoryUnit.h"

MemoryUnit::MemoryUnit(int *MEM, PipelineRegister *exmem, PipelineRegister *memwb) {
    MemoryUnit::MEM = MEM;
    MemoryUnit::exmem = exmem;
    MemoryUnit::memwb = memwb;
}

int MemoryUnit::memory() {
    *(memwb) = *(exmem);
    opcode opcode = exmem->cir.opcode;
    //Load
    if(opcode >= LD && opcode <= LDC) {
        memwb->MEMLoadData = MEM[exmem->ALUOut];
    }
    //Store
    else if(opcode == ST) {
        MEM[exmem->ALUOut] = exmem->Rd;
    }
    else if(opcode == STC) {
        MEM[exmem->ALUOut] = exmem->Ri;
    }

    return 0;
}