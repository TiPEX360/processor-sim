#include "WriteBackUnit.h"
#include <iostream>

WriteBackUnit::WriteBackUnit(int *RF, PipelineRegister *memwb) {
    WriteBackUnit::RF = RF;
    WriteBackUnit::memwb = memwb;
}

int WriteBackUnit::wb() {
    int tick = 0;
    opcode opcode = memwb->cir.opcode;
    if(opcode >= ADD && opcode <= XOR || (opcode == CMP)) {
        RF[memwb->Rd] = memwb->ALUOut;
        // std::cout << "RF[" << memwb->Rd << "]<-" << memwb->ALUOut << std::endl;
        tick++;
    }
    else if(opcode >= LD && opcode <= LDC) {
        RF[memwb->Rd] = memwb->MEMLoadData;
        // std::cout << "MEM" << memwb->Rd << memwb->MEMLoadData << std::endl;
        tick++;
    }
    
    memwb->active = false;

    return 0;
}