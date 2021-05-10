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
    nullEntry.opcode = Opcode::NOP;
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
    for(int i = 0; i < nextEntries.size(); i++) {
        RSEntry RSe = nextEntries[i];
        if(!RSe.executing) {
            if(RSe.RSd == e.id) {
                RSe.Rd = e.result;
                RSe.RSd = -1;
            }
            if(RSe.RSn == e.id) {
                RSe.Rn = e.result;
                RSe.RSn = -1;
            }
            if(RSe.RSi == e.id) {
                RSe.Ri = e.result;
                RSe.RSi = -1;
            }
            nextEntries[i] = RSe;
        }
    }
}

void ReservationStation::addEntry(int counter, Instr i) {
    if(nextEntries.size() < RS_SIZE) { 
        RSEntry n;
        Instr instr = i;
        n.executing = false;
        n.opcode = instr.opcode;
        n.bpc = instr.bpc;
        if(n.opcode == Opcode::ST) {
            bool found = false;
            //Check if previous instruction in bundle written to Rd
            for(int i = 0; i < SCALE_WIDTH; i++) {
                if(BWL[i].dest == instr.Rd && BWL[i].dest != -1) {
                    found = true;
                    n.RSd = BWL[i].ROBId;
                }
            }
            if(!found) { 
                int r = ROB->currentROB.size() - 1;
                while(!found && r >= 0) {
                    if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rd) {
                        found = true;
                        n.Rd = ROB->currentROB[r].result;
                        if(ROB->currentROB[r].ready) n.RSd = -1;
                        else {
                            n.RSd = ROB->currentROB[r].id;
                        }
                    }
                    r--;
                }
                if(!found) {
                    n.RSd = RF[instr.Rd].RS; // should ALWAYS be -1 if we got to here
                    n.Rd = RF[instr.Rd].value;
                }
            }
        }
        else {
            n.Rd = instr.Rd;
            n.RSd = -1;
        }
        //Rn Always register addressed (except STC then always available as 0) //STC?? think i meent LDC.
        bool found = false;
        for(int i = 0; i < SCALE_WIDTH; i++) {
                if(BWL[i].dest == instr.Rn && BWL[i].dest != -1) {
                    found = true;
                    n.RSn = BWL[i].ROBId;
                }
            }
        if(!found) { 
            int r = ROB->currentROB.size() - 1;
            while(!found && r >= 0) {
                if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rn) {
                    found = true;
                    n.Rn = ROB->currentROB[r].result;
                    if(ROB->currentROB[r].ready) n.RSn = -1;
                    else {
                        n.RSn = ROB->currentROB[r].id;
                    }
                }
                r--;
            }
            if(!found) {
                n.RSn = RF[instr.Rn].RS;
                n.Rn = RF[instr.Rn].value;
            }
        }
        found = false;
        if(n.opcode == Opcode::LDC || n.opcode == Opcode::JNZ || n.opcode == Opcode::J || n.opcode == Opcode::B || n.opcode == Opcode::BNZ|| n.opcode == Opcode::BZ|| n.opcode == Opcode::JZ) {
            n.RSn = -1;
            n.Rn = 0;
        }

        //Ri Immediate only if (instr.immediate == true)
        if(instr.immediate) {
            n.Ri = instr.Ri;
            n.RSi = -1;
        }
        else {
            found = false;
            for(int i = 0; i < SCALE_WIDTH; i++) {
                if(BWL[i].dest == instr.Ri && BWL[i].dest != -1) {
                    found = true;
                    n.RSi = BWL[i].ROBId;
                }
            }
            if(!found) { 
                int r = ROB->currentROB.size() - 1;
                while(!found && r >= 0) {
                    if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Ri) {
                        found = true;
                        n.Ri = ROB->currentROB[r].result;
                        if(ROB->currentROB[r].ready) n.RSi = -1;
                        else {
                            n.RSi = ROB->currentROB[r].id;
                        }
                    }
                    r--;
                }
                if(!found) {
                    n.RSi = RF[instr.Ri].RS; 
                    n.Ri = RF[instr.Ri].value;
                }
            }
        }
        if(n.opcode == Opcode::B || n.opcode == Opcode::J) {
            n.Ri = 0;
            n.RSi = -1;
        }

        //Exception for HALT
        if(n.opcode == Opcode::HALT) {
            n.RSn = -1;
            n.RSi = -1;
            n.RSd = -1;
        }
        //Add entry to ROB
        ROBEntry e = ROB->addEntry(n, RSID, instr.branchTaken); 
        n.ROBId = e.id;
        if(e.type == InstrType::REG) {
            BWL[counter].dest = e.dest;
            BWL[counter].ROBId = e.id;
        }

        nextEntries.push_back(n);
    }
    else {
        // std::cout << "ERROR: Tried to issue instruction to RS which is full." << std::endl;
    }
}


void ReservationStation::update() {
    currentEntries = nextEntries;
    // currentEntries.clear();
    // for(int i = 0; i < nextEntries.size(); i++) ReservationStation::currentEntries[i] = ReservationStation::nextEntries[i];
}

ReservationStation::ReservationStation(BundleWriteLog *BWL, std::vector<ReservationStation> *RSs, ReorderBuffer *ROB, Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount) {
    ReservationStation::RSs = RSs;
    ReservationStation::BWL = BWL;
    ReservationStation::RF = RF;
    ReservationStation::ROB = ROB;
    ReservationStation::issuedCurrent = issuedCurrent;
    ReservationStation::issuedNext = issuedNext;
    ReservationStation::type = type;
    ReservationStation::RSID = RSID;
    ReservationStation::RSCount = RSCount;
    // ReservationStation::rsex = rsex;
}