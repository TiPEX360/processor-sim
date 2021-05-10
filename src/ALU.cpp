#include "ALU.hpp"
#include "ReorderBuffer.hpp"
#include <iostream>

using namespace EU;

void ALU::tick() {
    //If instruction is being executed, progress it
    if(progress > 0) progress++;
    //No instruction is being executed. search RS for oldest RSEntry which is ready
    if(progress == 0) {
        processing = RS->getReadyEntry();
        switch(processing.opcode) {
            case Opcode::NOP:
                duration = 1;
                break;
            case Opcode::ADD:
                duration = 1;
                break;
            case Opcode::MUL:
                duration = 2;
                break;
            case Opcode::SUB:
                duration = 1;
                break;
            case Opcode::DIV:
                duration = 4;
                break;
            case Opcode::LSH:
                duration = 1;
                break;
            case Opcode::RSH:
                duration = 1;
                break;
            case Opcode::AND:
                duration = 1;
                break;
            case Opcode::OR:
                duration = 1;
                break;
            case Opcode::XOR:
                duration = 1;
                break;
            case Opcode::LDC:
                duration = 1;
                break;
            case Opcode::HALT:
                duration = 1;
                break;
            default:
                // std::cout << "ERROR: Invalid instruction in ALU. Opcode: " << (int)processing.opcode << std::endl;
                break;
        }
        progress++;

    }

    //Execute instruction if on final stage of execution
    if(progress == duration) {
        if(processing.opcode != Opcode::NOP) {
            nextOut.dest = processing.Rd;
            nextOut.type = InstrType::REG;
            nextOut.id = processing.ROBId;
        }
        switch(processing.opcode) {
            case Opcode::NOP:
                nextOut.dest = -1;
                nextOut.type = InstrType::NOP;
                nextOut.ready = false;
                nextOut.id = -1;
                nextOut.result = -1;
                nextOut.bpc = 0;
                break;
            case Opcode::ADD:
                nextOut.result = processing.Rn + processing.Ri;
                break;
            case Opcode::MUL:
                nextOut.result = processing.Rn * processing.Ri;
                break;
            case Opcode::SUB:
                nextOut.result = processing.Rn - processing.Ri;
                break;
            case Opcode::DIV:
                if(processing.Ri == 0) nextOut.result = INT32_MAX;
                else nextOut.result = (int)(processing.Rn / processing.Ri);
                break;
            case Opcode::LSH:
                nextOut.result = processing.Rn << processing.Ri;
                break;
            case Opcode::RSH:
                nextOut.result = processing.Rn >> processing.Ri;
                break;
            case Opcode::AND:
                nextOut.result = processing.Rn & processing.Ri;
                break;
            case Opcode::OR:
                nextOut.result = processing.Rn | processing.Ri;
                break;
            case Opcode::XOR:
                nextOut.result = processing.Rn ^ processing.Ri;
                break;
            case Opcode::LDC:
                nextOut.result = processing.Ri;
                break;
            case Opcode::HALT:
                nextOut.dest = -1;
                nextOut.type = InstrType::HALT;
                nextOut.ready = false;
                nextOut.result = -1;
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


ALU::ALU(ReservationStation *RS, ReorderBuffer *ROB) {
    ALU::RS = RS;
    ALU::ROB = ROB;
    progress = 0;
}

ALU::ALU() {
    progress = 0;
}