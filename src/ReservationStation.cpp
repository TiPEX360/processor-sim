#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include <iostream>

RSEntry ReservationStation::getReadyEntry() {
    // if(currentEntries.size() > 0) std::cout << (int)currentEntries[0].opcode << " RSd " << (int)currentEntries[0].RSd << " RSn: " << (int)currentEntries[0].RSn << " RSi: " << (int)currentEntries[0].RSi << std::endl;
    for(int e = 0; e < currentEntries.size(); e++) {
        RSEntry entry = currentEntries[e];
        if(!entry.executing && entry.RSd == -1 && entry.RSn == -1 && entry.RSi == -1) {
            nextEntries[e].executing = true;
            return nextEntries[e];
        }
    }
    RSEntry nullEntry;
    nullEntry.opcode = opcode::NOP;
    nullEntry.executing = true;
    return nullEntry;
}

int ReservationStation::updateEntry(ROBEntry e) {

}

void ReservationStation::addEntry(Instr i) {
    // std::cout << "Size: " << currentEntries.size() << std::endl;
    if(currentEntries.size() < RS_SIZE) { 
        RSEntry n;
        Instr instr = i;
        n.executing = false;
        n.opcode = instr.opcode;
        //Rd !!REGISTER VALUE FOR BELOW ELSE IMMEDIATE
        if((n.opcode == opcode::ST) || (n.opcode >= opcode::BLT && n.opcode <= opcode::B)) {
            bool found = false;
            int r = ROB->currentOccupied;
            while(ROB->currentOccupied > 0 && !found && r >= 0) {
                if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rd) {
                    found = true;
                    n.Rd = ROB->currentROB[r].result;
                    // ROBID robId = ROB->currentROB[r].id;
                    for(int RS = 0; RS < RS_COUNT; RS++) {
                        for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                            if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                                n.RSd = ((*RSs)[RS].RSID);
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
        //Rn !!Always register addressed
        bool found = false;
        int r = ROB->currentOccupied;
        while(ROB->currentOccupied > 0 && !found && r >= 0) {
            if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Rn) {
                found = true;
                n.Rn = ROB->currentROB[r].result;
                // ROBID robId = ROB->currentROB[r].id;
                for(int RS = 0; RS < RS_COUNT; RS++) {
                    for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                        if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                            n.RSn = ((*RSs)[RS].RSID);
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

        //Ri !!Immediate only if (instr.immediate == true)
        if(instr.immediate) {
            n.Ri = instr.Ri;
            n.RSi = -1;
        }
        else {
            bool found = false;
            int r = ROB->currentOccupied;
            while(ROB->currentOccupied > 0 && !found && r >= 0 && r != 0) {
                if(ROB->currentROB[r].type == InstrType::REG && ROB->currentROB[r].dest == instr.Ri) {
                    found = true;
                    n.Ri = ROB->currentROB[r].result;
                    // ROBID robId = ROB->currentROB[r].id;
                    for(int RS = 0; RS < RS_COUNT; RS++) {
                        for(int e = 0; e < (*RSs)[RS].currentEntries.size(); e++) {
                            if((*RSs)[RS].currentEntries[e].ROBId == ROB->currentROB[r].id) {
                                n.RSi = ((*RSs)[RS].RSID);
                            }
                        }
                    }
                }
                r--;
            }
            if(!found) {
                n.RSi = RF[instr.Ri].RS; //FIGURE OuT WHEN TO WRITE TO RF.RS Anser: in issue.
                n.Ri = RF[instr.Ri].value;
            }
        }
        //Add entry to ROB
        n.ROBId = ROB->addEntry(n, RSID);
        ROB->nextOccupied++;

        // //RSd
        // if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
        //     n.RSd = RF[n.Rd].RS;
        // }
        // else n.RSd = -1;
        // //RSn
        // // n.RSn = 
        // n.RSn = RF[instr.Rn].RS;
        
        // //RSi
        // if(instr.immediate) n.RSi = -1;
        // else n.RSi = RF[instr.Ri].RS;

        //Add reservation station entry
        // if(n.RSd == -1 && n.RSn == -1 && n.RSi == -1) n.ready = true;
        // else n.ready = false;
        nextEntries.push_back(n);
    }
    else {
        std::cout << "ERROR: Tried to issue instruction to RS which is full." << std::endl;
    }
}


void ReservationStation::tick() {

    //Search issued instructions to see if this RS has been assigned an instruction

    // for(int i = 0; i < issuedCurrent->size(); i++) {
    //     std::cout << (*issuedCurrent)[i].RSID << std::endl;
    //     if((*issuedCurrent)[i].RSID == RSID) {
    //         //New instruction found
    //         if(currentEntries.size() < RS_SIZE) {
    //             RSEntry n;
    //             Instr instr = (*issuedCurrent)[i];
    //             n. = true;
    //             n.opcode = instr.opcode;

    //             //Rd
    //             if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
    //                 if(RF[instr.Rd].RS == -1) n.Rd = RF[instr.Rd].value;
    //             }
    //             else n.Rd = instr.Rd;
    //             //Rn !!Always register addressed
    //             if(RF[instr.Rn].RS == -1) n.Rn = RF[instr.Rn].value;
    //             //Ri
    //             if(instr.immediate) n.Ri = instr.Ri;
    //             else if(RF[instr.Ri].RS == -1) n.Ri = RF[instr.Ri].value;
     
    //             //Add entry to ROB
    //             n.ROBId = ROB->addEntry(n);

    //             //RSd
    //             if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
    //                 n.RSd = RF[n.Rd].RS;
    //             }
    //             else n.RSd = -1;
    //             //RSn
    //             n.RSn = 
    //             // n.RSn = RF[instr.Rn].RS;
                
    //             //RSi
    //             if(instr.immediate) n.RSi = -1;
    //             else n.RSi = RF[instr.Ri].RS;

    //             if(n.RSd == -1 && n.RSn == -1 && n.RSi == -1) n.ready = true;
    //             else n.ready = false;


    //             //Add reservation station entry
    //             nextEntries.push_back(n);


    //             //remove taken instruction from issued
    //             issuedNext->erase(issuedNext->begin() + i);
    //             // idrs->active = false;
    //         }
    //         else {
    //             std::cout << "ERROR: Tried to issue instruction to RS which is full." << std::endl;
    //         }
    //     }
    // }
    std::cout << "RSID: " << RSID << " Entries: " << currentEntries.size() << std::endl;

    //Add potential new instruction to RS queue
    // if(idrs->active) {
    //     if(current.size() < RSCount) {
    //         RSEntry n;
    //         Instr i = idrs->cir;
    //         n.busy = true;
    //         n.opcode = i.opcode;

    //         //Rd
    //         if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
    //             if(RF[i.Rd].RS == -1) n.Rd = RF[i.Rd].value;
    //         }
    //         else n.Rd = i.Rd;
    //         //Rn
    //         if(RF[i.Rn].RS == -1) n.Rn = RF[i.Rn].value;
    //         //Ri
    //         if(i.immediate) n.Ri = i.Ri;
    //         else if(RF[i.Ri].RS == -1) n.Ri = RF[i.Ri].value;

    //         //RSd
    //         if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
    //             n.RSd = RF[n.Rd].RS;
    //         }
    //         else n.RSd = -1;
    //         //RSn
    //         n.RSd = RF[i.Rn].RS;
    //         //RSi
    //         if(i.immediate) n.RSn = -1;
    //         else n.RSn = RF[i.Ri].RS;

    //         if(n.RSd == -1 && n.RSn == -1 && n.RSi == -1) n.ready = true;
    //         else n.ready = false;

    //         //Add reservation station to queue
    //         next.push_back(n);

    //         //flag taken instruction
    //         idrs->active = false;
    //     }


    // }


}

void ReservationStation::update() {
    currentEntries = nextEntries;
    currentOccupied = nextOccupied;
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
    ReservationStation::currentOccupied = 0;
    // ReservationStation::rsex = rsex;
}