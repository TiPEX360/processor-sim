#include "WriteBackUnit.h"

WriteBackUnit::WriteBackUnit(int *RF, PipelineRegister *memwb) {
    WriteBackUnit::RF = RF;
    WriteBackUnit::memwb = memwb;
}

int WriteBackUnit::wb() {
    int tick = 0;
    opcode opcode = memwb->cir.opcode;

    if(opcode >= ADD && opcode <= XOR) {
        RF[memwb->Rd] = memwb->ALUOut;
        tick++;
    }
    else if(opcode == LD || opcode == LDC) {
        RF[memwb->Rd] = memwb->MEMLoadData;
        tick++;
    }
    
    memwb->active = false;

    return 0;
}