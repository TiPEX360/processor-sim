#include "LSU.hpp"
#include <iostream>

void LSU::tick() {
    //If no instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        // bool found = false;
        // for(int entry = 0; entry < RS_SIZE && !found; entry++) {
        //     if(RS->currentEntries[entry].ready) {
        //         found = true;
        processing = RS->getReadyEntry();

        switch(processing.opcode) {
            case NOP:
                duration = 1;
                break;
            case LD:
                duration = 3;
                break;
            case LDC:
                duration = 1;
                break;
            case ST:
                duration = 2;
                break;
            case STC:
                duration = 2;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    

    //If instruction is being executed, progress it
    if(progress > 0) progress++;

    if(progress == duration) {
        if(processing.opcode != NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case NOP:
                break;
            case LD:
                nextOut.dest = processing.Rd;
                nextOut.type = REG;
                nextOut.result = MEM[processing.Rn + processing.Ri];
                break;
            //THIS SHOULD GO IN ALU
            // case LDC:
            //     nextOut.dest = processing.Rd;
            //     nextOut.result = processing.Ri;
            //     duration = 1;
            //     break;
            case ST:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Rn + processing.Ri;
                nextOut.type = InstrType::MEM;
                duration = 1;
                break;
            case STC:
                nextOut.result = processing.Rd;
                nextOut.dest = processing.Ri;
                nextOut.type = InstrType::MEM;
                duration = 1;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << processing.opcode << std::endl;
                break;
        }
        progress = 0;
    }
}


LSU::LSU(uint32_t *MEM, ReservationStation *RS, ReorderBuffer *ROB) {
    LSU::RS = RS;
    LSU::ROB = ROB;
    LSU::MEM = MEM;
    progress = 0;
}

LSU::LSU() {
    progress = 0;
}
