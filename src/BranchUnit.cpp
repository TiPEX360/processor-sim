#include "BranchUnit.hpp"
#include <iostream>
using namespace EU;
void BranchUnit::tick() {
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
            case BLT:
                duration = 3;
                break;
            case BNZ:
                duration = 1;
                break;
            case J:
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
            case BLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                break;
            case BNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                break;
            case B:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
            case JLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
            case JNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
            case J:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << processing.opcode << std::endl;
                break;
        }

        //Take branch
        if(nextOut.result == 1) {
            nextPC->value = nextOut.dest;
        }

        progress = 0;
    }
}

BranchUnit::BranchUnit(Register *nextPC, ReservationStation *RS, ReorderBuffer *ROB) {
    BranchUnit::nextPC = nextPC;
    BranchUnit::RS = RS;
    BranchUnit::ROB = ROB;
}