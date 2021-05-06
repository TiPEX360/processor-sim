#include "LSU.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>
using namespace EU;
void LSU::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;

    //If no instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        // bool found = false;
        // for(int entry = 0; entry < RS_SIZE && !found; entry++) {
        //     if(RS->currentEntries[entry].ready) {
        //         found = true;
        processing = RS->getReadyEntry();
        // std::cout << "LSU Begin: " << (int)processing.opcode << std::endl;

        switch(processing.opcode) {
            case opcode::NOP:
                duration = 1;
                break;
            case opcode::LD:
                duration = 3;
                break;
            case opcode::LDC:
                duration = 1;
                break;
            case opcode::ST:
                duration = 2;
                break;
            case opcode::STC:
                duration = 2;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    


    if(progress == duration) {
        if(processing.opcode != opcode::NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::BRANCH;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                break;
            case opcode::LD:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::REG;
                nextOut.result = currentMEM[processing.Rn + processing.Ri];
                break;
            //THIS SHOULD GO IN ALU
            // case LDC:
            //     nextOut.dest = processing.Rd;
            //     nextOut.result = processing.Ri;
            //     duration = 1;
            //     break;
            case opcode::ST:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Rn + processing.Ri;
                nextOut.type = InstrType::MEM;
                // duration = 1;
                break;
            case opcode::STC:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Ri;
                nextOut.type = InstrType::MEM;
                // duration = 1;
                break;
        }
        // std::cout << "LSU Executed: " << (int)processing.opcode << std::endl;
        
        progress = 0;
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
