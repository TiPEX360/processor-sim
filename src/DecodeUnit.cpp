#include "DecodeUnit.h"
#include <iostream>

void DecodeUnit::tick() {
    //Try assign current instruction to a reservation station
    Instr i = currentFetched->front();
    std::cout << "Issuing: " << (int)i.opcode << std::endl;

    bool found = false;
    if(i.opcode == opcode::LDC || i.opcode >= opcode::ADD && i.opcode <= opcode::XOR) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::ALU && (*RSs)[RS].currentOccupied < RS_SIZE && ROB->currentOccupied < ROB_MAX) {
                //NEW WAY
                std::cout << "Found ALU RS" << std::endl;
                found = true;
                i.RSID = RS;
            
                (*RSs)[RS].addEntry(i);
                nextFetched->pop();
                (*RSs)[RS].nextOccupied++;
            }
        }
    }
    else if(i.opcode >= opcode::LD && i.opcode <= opcode::STC) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::LDST && (*RSs)[RS].currentOccupied < RS_SIZE && ROB->currentOccupied < ROB_MAX) {
            
                std::cout << "Found LDST RS" << std::endl;
                //NEW WAY
                found = true;
                i.RSID = RS;
            
                (*RSs)[RS].addEntry(i);
            std::cout << "here" << i.immediate << std::endl;
                nextFetched->pop();
                (*RSs)[RS].nextOccupied++;
            }
        }
    }
    else if(i.opcode >= opcode::BLT && i.opcode <= opcode::JNZ) {
        for(int RS = 0; RS < RS_COUNT && !found; RS++) {
            if((*RSs)[RS].type == RSType::BRANCH && (*RSs)[RS].currentOccupied < RS_SIZE && ROB->currentOccupied < ROB_MAX) {
            
                std::cout << "Found BRANCH RS" << std::endl;
                //NEW WAY
                found = true;
                i.RSID = RS;
            
                (*RSs)[RS].addEntry(i);
                nextFetched->pop();
                (*RSs)[RS].nextOccupied++;
            }
        }
    }
    else {
        //NOP
        std::cout << "NOP" << std::endl;
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



DecodeUnit::DecodeUnit(std::queue<Instr> *currentFetched, std::queue<Instr> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB) {
    // DecodeUnit::RF = RF;
    DecodeUnit::currentFetched = currentFetched;
    DecodeUnit::nextFetched = nextFetched;
    DecodeUnit::RSs = RSs;
    DecodeUnit::ROB = ROB;
    // DecodeUnit::ifid = ifid;
    // DecodeUnit::idrs = idrs;
    // DecodeUnit::RSs = RSs;

    // DecodeUnit::current.i = {NOP, 0, 0, 0, true};
}