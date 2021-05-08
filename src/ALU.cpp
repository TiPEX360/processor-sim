#include "ALU.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>

using namespace EU;

void ALU::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;
    //No instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        // bool found = false;
        // for(int entry = 0; entry < RS_SIZE && !found; entry++) {
        //     if(RS->currentEntries[entry].ready) {
        //         found = true;
        processing = RS->getReadyEntry();
        // std::cout << "ALU " << ALU::RS->RSID << " Begin: " << (int)processing.opcode << std::endl;
        switch(processing.opcode) {
            case opcode::NOP:
                duration = 1;
                break;
            case opcode::ADD:
                duration = 1;
                break;
            case opcode::MUL:
                duration = 2;
                break;
            case opcode::SUB:
                duration = 1;
                break;
            case opcode::DIV:
                duration = 4;
                break;
            case opcode::LSH:
                duration = 1;
                break;
            case opcode::RSH:
                duration = 1;
                break;
            case opcode::AND:
                duration = 1;
                break;
            case opcode::OR:
                duration = 1;
                break;
            case opcode::XOR:
                duration = 1;
                break;
            case opcode::LDC:
                duration = 1;
                break;
            case opcode::HALT:
                duration = 1;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in ALU. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;
        //     }
        // }
    }

    //Execute instruction if on final stage of execution
    if(progress == duration) {
        if(processing.opcode != opcode::NOP) {
            nextOut.dest = processing.Rd;
            nextOut.type = InstrType::REG;
            nextOut.id = processing.ROBId;
        }
        switch(processing.opcode) {
            case opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::NOP;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            case opcode::ADD:
                nextOut.result = processing.Rn + processing.Ri;
                break;
            case opcode::MUL:
                nextOut.result = processing.Rn * processing.Ri;
                break;
            case opcode::SUB:
                nextOut.result = processing.Rn - processing.Ri;
                break;
            case opcode::DIV:
                nextOut.result = (int)(processing.Rn / processing.Ri);
                // std::   nextOut.result << "Rd " << Rd << " Rn " << Rn << " Ri " << Ri << std::endl;
                break;
            case opcode::LSH:
                nextOut.result = processing.Rn << processing.Ri;
                break;
            case opcode::RSH:
                nextOut.result = processing.Rn >> processing.Ri;
                break;
            case opcode::AND:
                nextOut.result = processing.Rn & processing.Ri;
                break;
            case opcode::OR:
                nextOut.result = processing.Rn | processing.Ri;
                break;
            case opcode::XOR:
                nextOut.result = processing.Rn ^ processing.Ri;
                break;
            case opcode::LDC:
                nextOut.result = processing.Ri;
                break;
            case opcode::HALT:
                nextOut.dest = -1;
                nextOut.type = InstrType::HALT;
                nextOut.ready = false;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            default:
                break;
        }
        // std::cout << "ALU Executed: " << (int)processing.opcode << std::endl;
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


ALU::ALU(ReservationStation *RS, ReorderBuffer *ROB) {
    ALU::RS = RS;
    ALU::ROB = ROB;
    progress = 0;
}

ALU::ALU() {
    progress = 0;
}