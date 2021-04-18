#include "WriteBackUnit.h"
#include <iostream>

WriteBackUnit::WriteBackUnit(int *RF, PipelineRegister *memwb) {
    WriteBackUnit::RF = RF;
    WriteBackUnit::memwb = memwb;
}

int WriteBackUnit::wb() {
    int tick = 0;
    opcode opcode = memwb->cir.opcode;
    std::cout << "WB: " << opcode << std::endl;
    if(opcode >= ADD && opcode <= XOR) {
        RF[memwb->Rd] = memwb->ALUOut;
        tick++;
    }
    else if(opcode >= LD && opcode <= LDC) {
        //NOT HERE FOR LDC >:(
        RF[memwb->Rd] = memwb->MEMLoadData;
        std::cout << "RF[" << memwb->Rd << "]<-" << memwb->MEMLoadData << std::endl;
        std::cout << memwb->cir.immediate << std::endl;
        tick++;
    }
    
    memwb->active = false;

    return 0;
}