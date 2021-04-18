#include "MemoryUnit.h"
#include <iostream>

MemoryUnit::MemoryUnit(int *MEM, PipelineRegister *exmem, PipelineRegister *memwb) {
    MemoryUnit::MEM = MEM;
    MemoryUnit::exmem = exmem;
    MemoryUnit::memwb = memwb;
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
        std::cout << exmem->Rd << "->" << exmem->ALUOut << std::endl;
        tick++;
    }

    exmem->active = false;
    memwb->active = true;

    return 0;
}