#include "BranchUnit.hpp"
#include "ReorderBuffer.hpp"
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
            case opcode::NOP:
                duration = 1;
                break;
            case opcode::BLT:
                duration = 3;
                break;
            case opcode::BNZ:
                duration = 1;
                break;
            case opcode::J:
                duration = 2;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in LDST Unit. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    

    //If instruction is being executed, progress it
    if(progress > 0) progress++;

    if(progress == duration) {
        if(processing.opcode != opcode::NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case opcode::NOP:
                break;
            case opcode::BLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                break;
            case opcode::BNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                break;
            case opcode::B:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
            case opcode::JLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
            case opcode::JNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
            case opcode::J:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                break;
            default:
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