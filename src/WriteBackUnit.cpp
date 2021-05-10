#include "WriteBackUnit.h"
#include <iostream>

WriteBackUnit::WriteBackUnit(int *RF, PipelineRegister *memwb) {
    WriteBackUnit::RF = RF;
    WriteBackUnit::memwb = memwb;
}

int WriteBackUnit::wb() {
    opcode opcode = memwb->cir.opcode;
    if(opcode >= ADD && opcode <= XOR || (opcode == CMP)) {
        RF[memwb->Rd] = memwb->ALUOut;
    }
    else if(opcode >= LD && opcode <= LDC) {
        RF[memwb->Rd] = memwb->MEMLoadData;
        // std::cout << "RF[" << memwb->Rd << "]<-"<< memwb->MEMLoadData << std::endl;
    }
    // std::cout << "WB// Op: " << opcode << std::endl;
    memwb->active = false;

    return 0;
}