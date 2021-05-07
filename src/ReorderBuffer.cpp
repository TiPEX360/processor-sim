#include "ReorderBuffer.hpp"
#include "ReservationStation.h"
#include "ExecutionUnit.h"
#include <iostream>


ROBID ReorderBuffer::addEntry(RSEntry RSe, RSID RSID, int branchTaken) {
    ROBEntry e;
    e.ready = false;
    e.dest = -1;
    e.result = -1;
    if((RSe.opcode >= opcode::ADD && RSe.opcode <= opcode::XOR) || RSe.opcode == opcode::LDC || RSe.opcode == opcode::LD) {
        e.type =  InstrType::REG;
        e.dest = RSe.Rd;
        nextRF[e.dest].RS = RSID;
    }
    else if(RSe.opcode >= opcode::ST && RSe.opcode <= opcode::STC) e.type = InstrType::MEM;
    else if(RSe.opcode == opcode::HALT) e.type = InstrType::HALT;
    else {
        e.type = InstrType::BRANCH;
        e.dest = RSe.Rd;
        e.result = branchTaken;
    }

    int id = -1; //check this logic
    bool taken = true;
    while(taken) {
        id++;
        taken = false;
        for(int r = 0; r < nextROB.size(); r++) {
            if(id == nextROB[r].id) taken = true;
        }
    }
    e.id = id;

    nextROB.push_back(e);

    return e.id;
}

RSID ReorderBuffer::findRSIDByROBEntry(ROBEntry e) {
    bool found = false;
    int RSID = -1;
    for(int RS = 0; RS < RS_COUNT && !found; RS++) {
        for(int i = 0; i < (*RSs)[RS].currentEntries.size(); i++) {
            if((*RSs)[RS].currentEntries[i].ROBId == e.id) {
                return RS;
            }
        }
    }
    return -1;
}

int ReorderBuffer::updateEntry(int index, ROBEntry e) {
    if(nextROB[index].id == e.id) {
        nextROB[index] = e;
        int RSId = findRSIDByROBEntry(e);
        if(RSId == -1) std::cout << "ERROR: No RS looking for this ROBEntry!" << std::endl;
        if(e.type == InstrType::REG) {
            //Update RS
            for(int RS = 0; RS < RS_COUNT; RS++) {
                (*RSs)[RS].updateEntry(RSId, e);//cannot call function WTF
            }
        
        }
        else if(e.type == InstrType::MEM) {
           
        }
        else if(e.type == InstrType::BRANCH) {
            //do branch shit (BOOK SAYS NOTHING HAPPENS HERE)
            //decide if branch requires flush :/
            if(currentROB[index].result != e.result) {
                //branch mispredicted!
                nextROB[index].result = -1;
            }
        }
        else if(e.type == InstrType::HALT) {

        }
        nextROB[index].ready = true; 
        (*RSs)[RSId].removeEntry(e); //Free RS which produced result

    }
    else {
        std::cout << "ERROR: Tried to update invalid ROBEntry. ROBId: " << nextROB[index].id << " New Entry Id: " << e.id << std::endl;
        return 1;
    }
    return 0;
}

void ReorderBuffer::flush() {
    std::cout << "FLUSHING PIPE" << std::endl;
}

void ReorderBuffer::tick() {
    //WRITE RESULT
    for(int i = 0; i < EXEC_COUNT; i++) {
        ROBEntry out = (*EUs)[i]->currentOut;
        if(out.type != InstrType::NOP) {
            //Send CDB to waiting RSEntries not being executed
            for(int r = 0; r < currentROB.size(); r++) {
                if(currentROB[r].id == out.id && !currentROB[r].ready) {
                    std::cout << "Updating ROBEntry" << std::endl;
                    updateEntry(r, out);
                } 
            }
            //Send CDB to waiting RSEntries added in *this* cycle :o
            
            
        }
        
    }

    //COMMIT STEP
    if(currentROB.size() > 0 && currentROB[0].ready) {
        ROBEntry committing = currentROB[0];
        nextROB.erase(nextROB.begin()); //erase not working >:(

        if(committing.type == InstrType::REG) {
            if(committing.dest == 30) {
                std::cout << "ERROR: TRYING TO CHANGE PC IN REG INSTRUCTION" << std::endl;
                exit(1);
            }
            std::cout << "commiting REG" << std::endl;
            nextRF[committing.dest].value = committing.result;
            nextRF[committing.dest].RS = -1;

        }
        else if(committing.type == InstrType::MEM) {
            std::cout << "commiting MEM" << std::endl;
            nextMEM[committing.dest] = committing.result;
        }
        else if(committing.type == InstrType::BRANCH) {
            //do branch shit FLUSH HERE
            if(committing.result == -1) flush();
        }
        else if(committing.type == InstrType::HALT) {
            *halt = true;
        }
    }

}

void ReorderBuffer::update() {
    currentROB = nextROB;
}

ReorderBuffer::ReorderBuffer(bool *halt, std::array<ExecutionUnit *, EXEC_COUNT> *EUs, Register *nextRF, int32_t *nextMEM, std::array<ReservationStation, RS_COUNT> *RSs) {
    ReorderBuffer::EUs = EUs;
    ReorderBuffer::nextRF = nextRF;
    ReorderBuffer::nextMEM = nextMEM;
    ReorderBuffer::RSs = RSs;
    ReorderBuffer::halt = halt;
}

// ReorderBuffer::ReorderBuffer()  {};