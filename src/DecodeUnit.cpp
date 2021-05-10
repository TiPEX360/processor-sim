#include "DecodeUnit.h"
#include <iostream>

bool DecodeUnit::issueInstr(int counter, Instr n) {
    bool found = false;
    if(n.opcode == Opcode::HALT) {
        found = true;
        RSEntry e;
        e.opcode = Opcode::HALT;
        ROB->addEntry(e, -1, -1);
    }
    else if(n.opcode == Opcode::HALT || n.opcode == Opcode::LDC || n.opcode >= Opcode::ADD && n.opcode <= Opcode::XOR) {
        std::vector<int> availableRSs;
        for(int RS = 0; RS < RS_COUNT; RS++) {
            if((*RSs)[RS].type == RSType::ALU && (*RSs)[RS].nextEntries.size() < RS_SIZE && ROB->nextROB.size() < ROB_MAX) {
                availableRSs.push_back(RS);
            }
        }
        if(availableRSs.size() > 0) {
            int bestRS = availableRSs[0];
            for(int RS = 0; RS < availableRSs.size(); RS++) {
                if((*RSs)[availableRSs[RS]].currentEntries.size() < (*RSs)[bestRS].nextEntries.size()) bestRS = RS;
            }
            found = true;
            n.RSID = bestRS;
            (*RSs)[bestRS].addEntry(counter, n);
        }
    }
    else if(n.opcode >= Opcode::LD && n.opcode <= Opcode::STC) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::LDST && (*RSs)[RS].nextEntries.size() < RS_SIZE && ROB->nextROB.size() < ROB_MAX) {
                found = true;
                n.RSID = RS;
                (*RSs)[RS].addEntry(counter, n);
                // nextFetched->pop();
            }
        }
    }
    else if(n.opcode >= Opcode::BLT && n.opcode <= Opcode::JNZ) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::BRANCH && (*RSs)[RS].nextEntries.size() < RS_SIZE && ROB->nextROB.size() < ROB_MAX) {
                found = true;
                n.RSID = RS;
                (*RSs)[RS].addEntry(counter, n);
                // nextFetched->pop();
            }
        }
    }
    else {
        found = true;
    }
    
    return found;
}

void DecodeUnit::tick() {
    bool bundleBroken = false;
    std::deque<Instr> remainder;
    for(int i = 0; i < SCALE_WIDTH; i++) {
        if(!bundleBroken) {
            if(issueInstr(i, currentFetched->front())) {
                nextFetched->pop_front();
                currentFetched->pop_front();
            }
            else {
                bundleBroken = true;
            }
        }
        else if(bundleBroken) {
            remainder.push_back(currentFetched->front());
        }
    }

}


void DecodeUnit::update() {
    DecodeUnit::currentIssued = DecodeUnit::nextIssued;
}



DecodeUnit::DecodeUnit(std::deque<Instr> *currentFetched, std::deque<Instr> *nextFetched, std::vector<ReservationStation> *RSs, ReorderBuffer *ROB) {
    // DecodeUnit::RF = RF;
    DecodeUnit::currentFetched = currentFetched;
    DecodeUnit::nextFetched = nextFetched;
    DecodeUnit::RSs = RSs;
    DecodeUnit::ROB = ROB;
}