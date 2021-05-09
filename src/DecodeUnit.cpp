#include "DecodeUnit.h"
#include <iostream>

bool DecodeUnit::issueInstr(int counter, Instr n) {
    bool found = false;
    if(n.opcode == opcode::HALT || n.opcode == opcode::LDC || n.opcode >= opcode::ADD && n.opcode <= opcode::XOR) {
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
            // nextFetched->pop();
            std::cout << (*RSs)[bestRS].nextEntries.size();
        }
    }
    else if(n.opcode >= opcode::LD && n.opcode <= opcode::STC) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::LDST && (*RSs)[RS].nextEntries.size() < RS_SIZE && ROB->nextROB.size() < ROB_MAX) {
                found = true;
                n.RSID = RS;
                (*RSs)[RS].addEntry(counter, n);
                // nextFetched->pop();
            }
        }
    }
    else if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
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
        //NOP || HALT
        found = true;
        if(n.opcode == opcode::HALT) std::cout << "HALT" << std::endl;
        // nextFetched->pop();
    }
    
    return found;
}

void DecodeUnit::tick() {
    bool bundleBroken = false;
    std::vector<Instr> remainder;
    for(int i = 0; i < 4; i++) {
        //  if(bundleBroken && (*currentFetched)[i].front().opcode != opcode::NOP) {
        if(bundleBroken) {
            remainder.push_back((*currentFetched)[i].front());
        }
        else if(!issueInstr(i, (*currentFetched)[i].front())) {
            bundleBroken = true;
        }
        (*nextFetched)[i].pop();
    }

    //Reconstruct issue queue :: Logic worth checking
    std::array<std::queue<Instr>, 4> newQueue;
    for(int i = 0; i < remainder.size(); i++) {
        newQueue[i].push(remainder[i]);
        for(int j = 0; j < (*currentFetched)[i].size(); j++) {
            newQueue[i].push((*currentFetched)[i].front());
            (*nextFetched)[i].pop();
        }
    }
    for(int i = remainder.size(); i < 4; i++) {
        for(int j = 0; j < (*nextFetched)[i].size(); j++) {
            newQueue[i].push((*nextFetched)[i].front());
            (*nextFetched)[i].pop();
        }
    }


    for(int i = 0; i < 4; i++) (*nextFetched)[i] = newQueue[i];
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