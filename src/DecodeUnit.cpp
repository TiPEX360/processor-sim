#include "DecodeUnit.h"
#include <iostream>

bool DecodeUnit::issueInstr(int counter, Instr n) {
    bool found = false;
    if(n.opcode == opcode::HALT) {
        found = true;
        RSEntry e;
        e.opcode = opcode::HALT;
        ROB->addEntry(e, -1, -1);
    }
    else if(n.opcode == opcode::HALT || n.opcode == opcode::LDC || n.opcode >= opcode::ADD && n.opcode <= opcode::XOR) {
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
            // std::cout << (*RSs)[bestRS].nextEntries.size();
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
        // if(n.opcode == opcode::HALT) std::cout << "HALT" << std::endl;
        // nextFetched->pop();
    }
    
    return found;
}

void DecodeUnit::tick() {
    bool bundleBroken = false;
    std::deque<Instr> remainder;
    for(int i = 0; i < SCALE_WIDTH; i++) {
        if(!bundleBroken) {
            if(issueInstr(i, currentFetched->front())) {
                // std::cout << "Issued " << (int)currentFetched->front().opcode << std::endl;
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

    // int size = remainder.size();
    // for(int i = 0; i < size; i++) {
    //     nextFetched->push_front(remainder.back());
    //     remainder.pop_back();
    // }

    // (*nextFetched) = newNextFetched;
}


void DecodeUnit::update() {
    DecodeUnit::currentIssued = DecodeUnit::nextIssued;
}



DecodeUnit::DecodeUnit(std::deque<Instr> *currentFetched, std::deque<Instr> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB) {
    // DecodeUnit::RF = RF;
    DecodeUnit::currentFetched = currentFetched;
    DecodeUnit::nextFetched = nextFetched;
    DecodeUnit::RSs = RSs;
    DecodeUnit::ROB = ROB;
}