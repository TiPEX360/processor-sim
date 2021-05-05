#include "ReorderBuffer.hpp"

ROBID ReorderBuffer::addEntry(RSEntry RSe) {
    ROBEntry e;
    e.ready = false;
    e.dest = -1;
    e.result = -1;
    if(RSe.opcode >= ADD && RSe.opcode <= XOR || RSe.opcode == LDC) e.type =  REG;
    else if(RSe.opcode >= LD && RSe.opcode <= STC) e.type = InstrType::MEM;
    else e.type = BRANCH;

    int id = -1; //check this logic
    bool exists = true;
    while(exists) {
        id++;
        exists = false;
        for(int r = 0; r < nextROB.size(); r++) {
            if(id == nextROB[r].id) exists = true;
        }
    }
    e.id = id;

    nextROB.push_back(e);

    return e.id;
}

//Write Result Step
void ReorderBuffer::tick() {
    
}

void ReorderBuffer::update() {
    currentROB = nextROB;
}

ReorderBuffer::ReorderBuffer(Register *RF, int32_t *MEM, std::vector<ReservationStation> *RSs) {
    ReorderBuffer::RF = RF;
    ReorderBuffer::MEM = MEM;
    ReorderBuffer::RSs = RSs;
}

ReorderBuffer::ReorderBuffer()  {};