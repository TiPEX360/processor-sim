#include "DecodeUnit.h"
#include <iostream>

bool DecodeUnit::issueInstr(Instr n) {
    bool found = false;
    if(n.opcode == opcode::HALT || n.opcode == opcode::LDC || n.opcode >= opcode::ADD && n.opcode <= opcode::XOR) {
        std::vector<int> availableRSs;
        for(int RS = 0; RS < RS_COUNT; RS++) {
            if((*RSs)[RS].type == RSType::ALU && (*RSs)[RS].currentEntries.size() < RS_SIZE && ROB->currentROB.size() < ROB_MAX) {
                availableRSs.push_back(RS);
            }
        }
        if(availableRSs.size() > 0) {
            int bestRS = availableRSs[0];
            for(int RS = 0; RS < availableRSs.size(); RS++) {
                if((*RSs)[availableRSs[RS]].currentEntries.size() < (*RSs)[bestRS].currentEntries.size()) bestRS = RS;
            }
            found = true;
            n.RSID = bestRS;
            (*RSs)[bestRS].addEntry(n);
            // nextFetched->pop();
            std::cout << (*RSs)[bestRS].nextEntries.size();
        }
    }
    else if(n.opcode >= opcode::LD && n.opcode <= opcode::STC) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::LDST && (*RSs)[RS].currentEntries.size() < RS_SIZE && ROB->currentROB.size() < ROB_MAX) {
                found = true;
                n.RSID = RS;
                (*RSs)[RS].addEntry(n);
                // nextFetched->pop();
            }
        }
    }
    else if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::BRANCH && (*RSs)[RS].currentEntries.size() < RS_SIZE && ROB->currentROB.size() < ROB_MAX) {
                found = true;
                n.RSID = RS;
                (*RSs)[RS].addEntry(n);
                // nextFetched->pop();
            }
        }
    }
    else {
        //NOP || HALT
        found = true;
        if(n.opcode == opcode::HALT) std::cout << "HALT" << std::endl;
        // nextFetched->pop();
    }
    
    return found;
}

void DecodeUnit::tick() {
    //Try assign current instruction to a reservation station
    Instr n[4];
    for(int i = 0; i < 4; i++) {
        n[i] = (*currentFetched)[4].front();
        if(issueInstr(n[i])) {
            (*nextFetched)[i].pop();
        }
    }
}


void DecodeUnit::update() {
    DecodeUnit::currentIssued = DecodeUnit::nextIssued;
}



DecodeUnit::DecodeUnit(std::array<std::queue<Instr>, 4> *currentFetched, std::array<std::queue<Instr>, 4> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB) {
    // DecodeUnit::RF = RF;
    DecodeUnit::currentFetched = currentFetched;
    DecodeUnit::nextFetched = nextFetched;
    DecodeUnit::RSs = RSs;
    DecodeUnit::ROB = ROB;
}