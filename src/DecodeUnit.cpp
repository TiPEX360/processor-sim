#include "DecodeUnit.h"
#include <iostream>

void DecodeUnit::tick() {
    //Try assign current instruction to a reservation station
    Instr i = currentFetched->front();
    std::cout << "Issuing: " << i.opcode << std::endl;
    // bool issued = false;
    // if(i.instr.opcode == NOP) {
    //     for(int RS = 0; RS < RS_COUNT; RS++) {
    //         // IssuedInstr issued = {i, RS};
    //         current.push_back(issued);
    //         // idrs[rs].active = true; //handle noops in RS
    //         // issued = true;
    //         instrQueue.pop();
    //     }
    // }
    bool found = false;
    if(i.opcode >= ADD && i.opcode <= XOR) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == ALU && (*RSs)[RS].currentEntries.size() < RS_SIZE) {
                // std::queue<InstrData> current;
                found = true;
                Instr issued = i;
                issued.RSID = RS;
                nextIssued.push_back(issued);
                nextFetched->pop();
                // idrs[rs].cir = i;
                // idrs[rs].active = true;
                // issued = true;
            }
        }
    }
    else if(i.opcode >= LD && i.opcode <= STC) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == LDST && (*RSs)[RS].currentEntries.size() < RS_SIZE) {
                found = true;
                Instr issued = i;
                issued.RSID = RS;
                std::cout << "LDST RS: " << RS << std::endl;
                nextIssued.push_back(issued);
                // std::cout << "NextFetch: " << nextFetched->front().opcode << std::endl;
                nextFetched->pop();
                // std::cout << "NextFetch: " << nextFetched->front().opcode << std::endl;
                // idrs[rs].cir = i;
                // idrs[rs].active = true;
                // issued = true;
            }
        }
    }
    else if(i.opcode >= BLT && i.opcode <= JNZ) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == BRANCH && (*RSs)[RS].currentEntries.size() < RS_SIZE) {
                found = true;
                Instr issued = i;
                issued.RSID = RS;
                nextIssued.push_back(issued);
                nextFetched->pop();
                // idrs[rs].cir = i;
                // idrs[rs].active = true;
                // issued = true;
            }
        }
    }
    else {
        //NOP
        nextFetched->pop();
    }

    //Take next instruction from fetch queue
    // if(issued) {
    //     next.i = instrQueue.front();
    //     instrQueue.pop();
    // } else {
        //block 
        // instrQueue.pop();
}




void DecodeUnit::update() {
    DecodeUnit::currentIssued = DecodeUnit::nextIssued;
}



DecodeUnit::DecodeUnit(std::queue<Instr> *currentFetched, std::queue<Instr> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs) {
    // DecodeUnit::RF = RF;
    DecodeUnit::currentFetched = currentFetched;
    DecodeUnit::nextFetched = nextFetched;
    DecodeUnit::RSs = RSs;
    // DecodeUnit::ifid = ifid;
    // DecodeUnit::idrs = idrs;
    // DecodeUnit::RSs = RSs;

    // DecodeUnit::current.i = {NOP, 0, 0, 0, true};
}