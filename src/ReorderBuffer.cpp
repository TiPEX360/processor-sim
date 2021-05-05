#include "ReorderBuffer.hpp"
#include <iostream>

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

int ReorderBuffer::updateEntry(int index, ROBEntry e) {
    if(nextROB[index].id == e.id) {
        //Update ROB
        nextROB[index] = e;

        if(e.type == REG) {
            //Update RS
            for(int RS = 0; RS < RS_COUNT; RS++) (*RSs)[RS].updateEntry(e); // implement this (and mark RS as available)
            nextROB[index].ready;    
        }
        else if(e.type == InstrType::MEM) {
            nextROB[index].ready; //assuming both address and value of store is available.
        }
        else if(e.type == InstrType::BRANCH) {
            //do branch shit
        }

    }
    else {
        std::cout << "ERROR: Tried to update invalid ROBEntry. ROBId: " << nextROB[index].id << " New Entry Id: " << e.id << std::endl;
        return 1;
    }
    return 0;
}

void ReorderBuffer::tick() {
    //Check all execution units for new outputs (is ROB already 'ready')
    for(int i = 0; i < EXEC_COUNT; i++) {
        ROBEntry out = (*EUs)[i]->currentOut;
        for(int r = 0; r < currentROB.size(); r++) {
            if(currentROB[r].id == out.id && !currentROB[r].ready) {
                updateEntry(r, out); //WRITE RESULT STEP
            } 
        }
    }

    //COMMIT STEP
    if(currentROB[0].ready) {
        ROBEntry committing = currentROB[0];
        nextROB.erase(nextROB.begin());

        if(committing.type == REG) {
            RF[committing.dest].value = committing.result;
            RF[committing.dest].RS = -1;

        }
        else if(committing.type == InstrType::MEM) {
            MEM[committing.dest] = committing.result;
        }
        else if(committing.type == InstrType::BRANCH) {
            //do branch shit
        }
    }

}

void ReorderBuffer::update() {
    currentROB = nextROB;
}

ReorderBuffer::ReorderBuffer(std::vector<ExecutionUnit *> *EUs, Register *RF, int32_t *MEM, std::vector<ReservationStation> *RSs) {
    ReorderBuffer::RF = RF;
    ReorderBuffer::MEM = MEM;
    ReorderBuffer::RSs = RSs;
    ReorderBuffer::EUs = EUs;
}

ReorderBuffer::ReorderBuffer()  {};