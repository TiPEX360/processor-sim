#include "BranchUnit.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>
using namespace EU;
void BranchUnit::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;
    //If no instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        // bool found = false;
        // for(int entry = 0; entry < RS_SIZE && !found; entry++) {
        //     if(RS->currentEntries[entry].ready) {
        //         found = true;
        processing = RS->getReadyEntry();
        // std::cout << "BU Begin: " << (int)processing.opcode << std::endl;

        switch(processing.opcode) {
            case opcode::NOP:
                duration = 1;
                break;
            case opcode::BLT:
                duration = 1;
                break;
            case opcode::BNZ:
                duration = 1;
                break;
            case opcode::B:
                duration = 1;
                break;
            case opcode::JLT:
                duration = 1;
                break;
            case opcode::JNZ:
                duration = 1;
                break;
            case opcode::J:
                duration = 1;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in BU. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    



    if(progress == duration) {
        if(processing.opcode != opcode::NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::NOP;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            case opcode::BLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case opcode::BNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case opcode::B:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                nextOut.bpc = processing.bpc;
                break;
            case opcode::JLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case opcode::JNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case opcode::J:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                nextOut.bpc = processing.bpc;
                break;
            default:
                break;
        }

        //Take branch
        // if(nextOut.result == 1) {
        //     nextPC->value = nextOut.dest;//FFS......
        // }
        // std::cout << "BU Executed: " << (int)processing.opcode << std::endl;
        
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

BranchUnit::BranchUnit(Register *nextPC, ReservationStation *RS, ReorderBuffer *ROB) {
    BranchUnit::nextPC = nextPC;
    BranchUnit::RS = RS;
    BranchUnit::ROB = ROB;
}