#include "ALU.hpp"
#include <iostream>

void ALU::tick() {
    //No instruction is being executed. search RS for oldest RSEntry which is ready
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
            case ADD:
                duration = 1;
                break;
            case MUL:
                duration = 2;
                break;
            case SUB:
                duration = 1;
                break;
            case DIV:
                duration = 4;
                break;
            case LSH:
                duration = 1;
                break;
            case RSH:
                duration = 1;
                break;
            case AND:
                duration = 1;
                break;
            case OR:
                duration = 1;
                break;
            case XOR:
                duration = 1;
                break;
            case LDC:
                duration = 1;
                break;
            default:
                std::cout << "ERROR: Invalid instruction in ALU. Opcode: " << processing.opcode << std::endl;
                break;
        }
        progress++;
        //     }
        // }
    }

    //If instruction is being executed, progress it
    if(progress > 0) progress++;

    //Execute instruction if on final stage of execution
    if(progress == duration) {
        if(processing.opcode != NOP) {
            nextOut.dest = processing.Rd;
            nextOut.type = REG;
            nextOut.id = processing.ROBId;
        }
        switch(processing.opcode) {
            case NOP:
                break;
            case ADD:
                nextOut.result = processing.Rn + processing.Ri;
                break;
            case MUL:
                nextOut.result = processing.Rn * processing.Ri;
                break;
            case SUB:
                nextOut.result = processing.Rn - processing.Ri;
                break;
            case DIV:
                nextOut.result = (int)(processing.Rn / processing.Ri);
                // std::   nextOut.result << "Rd " << Rd << " Rn " << Rn << " Ri " << Ri << std::endl;
                break;
            case LSH:
                nextOut.result = processing.Rn << processing.Ri;
                break;
            case RSH:
                nextOut.result = processing.Rn >> processing.Ri;
                break;
            case AND:
                nextOut.result = processing.Rn & processing.Ri;
                break;
            case OR:
                nextOut.result = processing.Rn | processing.Ri;
                break;
            case XOR:
                nextOut.result = processing.Rn ^ processing.Ri;
                break;
            case LDC:
                nextOut.result = processing.Ri;
                break;
        }
        progress = 0;
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