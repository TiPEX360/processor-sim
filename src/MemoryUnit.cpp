#include "MemoryUnit.h"
#include <iostream>

MemoryUnit::MemoryUnit(int *pc, int *MEM, PipelineRegister *exmem, PipelineRegister *memwb) {
    MemoryUnit::MEM = MEM;
    MemoryUnit::exmem = exmem;
    MemoryUnit::memwb = memwb;
    MemoryUnit::pc = pc;
}

int MemoryUnit::memory() {

    *(memwb) = *(exmem);
    opcode opcode = exmem->cir.opcode;
    //Load
    if(opcode == LD) {
        memwb->MEMLoadData = MEM[exmem->ALUOut];
    }
    else if(opcode == LDC) {
        memwb->MEMLoadData = exmem->ALUOut;

    }
    //Store
    else if(opcode == ST) {
        MEM[exmem->ALUOut] = exmem->Rd;
    }
    else if(opcode == STC) {
        MEM[exmem->ALUOut] = exmem->Rd;
    }
    else if((opcode >= BLT) && (opcode <= JNZ) && exmem->cond) {
        // ifid->cir = INSTR[exmem->ALUOut];
        // ifid->npc = exmem->ALUOut; //FETCH CORRECT INSTR HERE
        (*pc) = exmem->ALUOut;
    }
    std::cout << "Memory// Op: " << opcode << " cond: " << exmem->cond << std::endl;
    memwb->active = true;
    exmem->active = false;

    return 0;
}