#include "LSU.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>
using namespace EU;
void LSU::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;

    //If no instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        processing = RS->getReadyEntry();

        switch(processing.opcode) {
            case Opcode::NOP:
                duration = 1;
                break;
            case Opcode::LD:
                duration = 3;
                break;
            case Opcode::ST:
                duration = 2;
                break;
            case Opcode::STC:
                duration = 2;
                break;
            default:
                // std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    


    if(progress == duration) {
        if(processing.opcode != Opcode::NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case Opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::BRANCH;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            case Opcode::LD:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::REG;
                nextOut.result = currentMEM[processing.Rn + processing.Ri];
                nextOut.bpc = 0;
                break;
            case Opcode::ST:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Rn + processing.Ri;
                nextOut.type = InstrType::MEM;
                nextOut.bpc = 0;
                break;
            case Opcode::STC:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Ri;
                nextOut.type = InstrType::MEM;
                nextOut.bpc = 0;
                break;
            default:
                break;
        }

        progress = 0;
    }
    else {
        //Default null output
        nextOut.dest = -1;
        nextOut.type = InstrType::NOP;
        nextOut.ready = false;
        nextOut.id = -1;
        nextOut.result = -1;
        nextOut.bpc = 0;
    }
}


LSU::LSU(int32_t *currentMEM, ReservationStation *RS, ReorderBuffer *ROB) {
    LSU::RS = RS;
    LSU::ROB = ROB;
    LSU::currentMEM = currentMEM;
    progress = 0;
}

LSU::LSU() {
    progress = 0;
}
