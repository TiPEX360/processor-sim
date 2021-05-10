#include "BranchUnit.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>
using namespace EU;
void BranchUnit::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;
    //If no instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        processing = RS->getReadyEntry();

        switch(processing.opcode) {
            case Opcode::NOP:
                duration = 1;
                break;
            case Opcode::BLT:
                duration = 1;
                break;
            case Opcode::BZ:
                duration = 1;
                break;
            case Opcode::BNZ:
                duration = 1;
                break;
            case Opcode::B:
                duration = 1;
                break;
            case Opcode::JLT:
                duration = 1;
                break;
            case Opcode::JZ:
                duration = 1;
                break;
            case Opcode::JNZ:
                duration = 1;
                break;
            case Opcode::J:
                duration = 1;
                break;
            default:
                // std::cout << "ERROR: Invalid instruction in BU. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
    }
    



    if(progress == duration) {
        if(processing.opcode != Opcode::NOP) nextOut.id = processing.ROBId;
        switch(processing.opcode) {
            case Opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::NOP;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            case Opcode::BLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::BZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri == 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::BNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::B:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::JLT:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Rn < processing.Ri ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::JZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri == 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::JNZ:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = processing.Ri != 0 ? 1 : 0;
                nextOut.bpc = processing.bpc;
                break;
            case Opcode::J:
                nextOut.dest = processing.Rd;
                nextOut.type = InstrType::BRANCH;
                nextOut.result = 1;
                nextOut.bpc = processing.bpc;
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

BranchUnit::BranchUnit(Register *nextPC, ReservationStation *RS, ReorderBuffer *ROB) {
    BranchUnit::nextPC = nextPC;
    BranchUnit::RS = RS;
    BranchUnit::ROB = ROB;
}