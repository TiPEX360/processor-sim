#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include <iostream>

RSEntry ReservationStation::getReadyEntry() {
    // if(currentEntries.size() > 0) std::cout << (int)currentEntries[0].opcode << " RSd " << (int)currentEntries[0].RSd << " RSn: " << (int)currentEntries[0].RSn << " RSi: " << (int)currentEntries[0].RSi << std::endl;
    for(int e = 0; e < currentEntries.size(); e++) {
        if(!currentEntries[e].executing && currentEntries[e].RSd == -1 && currentEntries[e].RSn == -1 && currentEntries[e].RSi == -1) {
            nextEntries[e].executing = true;
            return nextEntries[e];
        }
    }
    RSEntry nullEntry;
    nullEntry.opcode = opcode::NOP;
    nullEntry.executing = true;
    return nullEntry;
}

int ReservationStation::removeEntry(ROBEntry e) {
    for(int i = 0; i < currentEntries.size(); i++) {
        if(currentEntries[i].ROBId == e.id) {
            nextEntries.erase(nextEntries.begin() + i);
            return 0;
        }
    }
    return 1;
}

void ReservationStation::updateEntry(int RS, ROBEntry e) { //TODO: effectively forwarding. Once this ROBEntry is 'current' (next cycle), RS will need to check for operands again!!! THIS IS IT! WE ARE NOT RECHECKING OpERANDS EVERY CYCLE!!!
    //TODO: if executing == true, ignore it!!!
    //For each RSEntry check all operands if waiting for result from RS this robid is linked to
    for(int i = 0; i < currentEntries.size(); i++) {
        RSEntry RSe = currentEntries[i];
        if(!RSe.executing) {
            if(RSe.RSd == RS) {
                RSe.Rd = e.result;
                RSe.RSd = -1;
            }
            if(RSe.RSn == RS) {
                RSe.Rn = e.result;
                RSe.RSn = -1;
            }
            if(RSe.RSi == RS) {
                RSe.Ri = e.result;
                RSe.RSi = -1;
            }
            nextEntries[i] = RSe;
        }
    }
    //Update RSs added in this cycle, only available in next cycle :o
    //Consider putting this in decode stage
    for(int i = 0; i < nextEntries.size(); i++) {
        RSEntry RSe = nextEntries[i];
        if(!RSe.executing) {
            if(RSe.RSd == RS) {
                RSe.Rd = e.result;
                RSe.RSd = -1;
            }
            if(RSe.RSn == RS) {
                RSe.Rn = e.result;
                RSe.RSn = -1;
            }
            if(RSe.RSi == RS) {
                RSe.Ri = e.result;
                RSe.RSi = -1;
            }
            nextEntries[i] = RSe;
        }
    }
}

void ReservationStation::addEntry(Instr i) {
    if(currentEntries.size() < RS_SIZE) { 
        RSEntry n;
        Instr instr = i;
        n.executing = false;
        n.opcode = instr.opcode;
        //Rd REGISTER VALUE FOR BELOW ELSE IMMEDIATE
        // if((n.opcode == opcode::ST) || (n.opcode >= opcode::BLT && n.opcode <= opcode::B)) {
        if(n.opcode == opcode::ST) {
            bool found = false;
            int r = ROB->currentROB.size();
            while(ROB->currentROB.size() > 0 && !found && r >= 0) {
                if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rd) {
                    found = true;
                    n.Rd = ROB->currentROB[r].result;
                    if(ROB->currentROB[r].ready) n.RSd = -1;
                    else {
                        for(int RS = 0; RS < RS_COUNT; RS++) {
                            for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                                if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                                    n.RSd = ((*RSs)[RS].RSID);
                                }
                            }
                        }
                    }
                }
                r--;
            }
            if(!found) {
                n.RSd = RF[instr.Rd].RS;
                n.Rd = RF[instr.Rd].value;
            }
        }
        else {
            n.Rd = instr.Rd;
            n.RSd = -1;
        }
        //Rn Always register addressed (except STC then always available as 0)
        bool found = false;
        int r = ROB->currentROB.size();
        while(ROB->currentROB.size() > 0 && !found && r >= 0) {
            if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rn) {
                found = true;
                n.Rn = ROB->currentROB[r].result;
                if(ROB->currentROB[r].ready) n.RSn = -1;
                else {
                    for(int RS = 0; RS < RS_COUNT; RS++) {
                        for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                            if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                                n.RSn = ((*RSs)[RS].RSID);
                            }
                        }
                    }
                }
            }
            r--;
        }
        if(!found) {
            n.RSn = RF[instr.Rn].RS;
            n.Rn = RF[instr.Rn].value;
        }
        found = false;
        if(n.opcode == opcode::LDC || n.opcode == opcode::JNZ || n.opcode == opcode::J || n.opcode == opcode::B || n.opcode == opcode::BNZ) {
            n.RSn = -1;
            n.Rn = 0;
        }

        //Ri Immediate only if (instr.immediate == true)
        if(instr.immediate) {
            n.Ri = instr.Ri;
            n.RSi = -1;
        }
        else {
            bool found = false;
            int r = ROB->currentROB.size();
            while(ROB->currentROB.size() > 0 && !found && r >= 0) {
                if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Ri) {
                    found = true;
                    n.Ri = ROB->currentROB[r].result;
                    if(ROB->currentROB[r].ready) n.RSi = -1;
                    else {
                        for(int RS = 0; RS < RS_COUNT; RS++) {
                            for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                                if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                                    n.RSi = ((*RSs)[RS].RSID);
                                }
                            }
                        }
                    }
                }
                r--;
            }
            if(!found) {
                n.RSi = RF[instr.Ri].RS; 
                n.Ri = RF[instr.Ri].value;
            }
        }
        if(n.opcode == opcode::B || n.opcode == opcode::J) {
            n.Ri = 0;
            n.RSi = -1;
        }

        //Exception for HALT
        if(n.opcode == opcode::HALT) {
            n.RSn = -1;
            n.RSi = -1;
            n.RSd = -1;
        }
        //Add entry to ROB
        n.ROBId = ROB->addEntry(n, RSID, i.branchTaken);

        nextEntries.push_back(n);
    }
    else {
        std::cout << "ERROR: Tried to issue instruction to RS which is full." << std::endl;
    }
}


void ReservationStation::update() {
    currentEntries = nextEntries;
    // currentEntries.clear();
    // for(int i = 0; i < nextEntries.size(); i++) ReservationStation::currentEntries[i] = ReservationStation::nextEntries[i];
}

ReservationStation::ReservationStation(std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB, Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount) {
    ReservationStation::RSs = RSs;
    ReservationStation::RF = RF;
    ReservationStation::ROB = ROB;
    ReservationStation::issuedCurrent = issuedCurrent;
    ReservationStation::issuedNext = issuedNext;
    ReservationStation::type = type;
    ReservationStation::RSID = RSID;
    ReservationStation::RSCount = RSCount;
    // ReservationStation::rsex = rsex;
}