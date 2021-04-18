#include "WriteBackUnit.h"

WriteBackUnit::WriteBackUnit(int *RF, PipelineRegister *memwb) {
    WriteBackUnit::RF = RF;
    WriteBackUnit::memwb = memwb;
}

int WriteBackUnit::wb() {
    opcode opcode = memwb->cir.opcode;

    if(opcode >= ADD && opcode <= XOR) {
        RF[memwb->Rd] = memwb->ALUOut;
    }
    else if(opcode == LD || opcode == LDC) {
        RF[memwb->Rd] = memwb->MEMLoadData;
    }
    return 0;
}