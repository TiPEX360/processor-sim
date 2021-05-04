#include "ReservationStation.h"
#include <iostream>

void ReservationStation::tick() {

    //Search issued instructions to see if this RS has been assigned an instruction

    for(int i = 0; i < issuedCurrent->size(); i++) {
        std::cout << (*issuedCurrent)[i].RSID << std::endl;
        if((*issuedCurrent)[i].RSID == RSID) {
            //New instruction found
            if(currentEntries.size() < RS_SIZE) {
                RSEntry n;
                Instr instr = (*issuedCurrent)[i];
                n.busy = true;
                n.opcode = instr.opcode;

                //Rd
                if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
                    if(RF[instr.Rd].RS == -1) n.Rd = RF[instr.Rd].value;
                }
                else n.Rd = instr.Rd;
                //Rn
                if(RF[instr.Rn].RS == -1) n.Rn = RF[instr.Rn].value;
                //Ri
                if(instr.immediate) n.Ri = instr.Ri;
                else if(RF[instr.Ri].RS == -1) n.Ri = RF[instr.Ri].value;

                //RSd
                if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
                    n.RSd = RF[n.Rd].RS;
                }
                else n.RSd = -1;
                //RSn
                n.RSd = RF[instr.Rn].RS;
                //RSi
                if(instr.immediate) n.RSn = -1;
                else n.RSn = RF[instr.Ri].RS;

                if(n.RSd == -1 && n.RSn == -1 && n.RSi == -1) n.ready = true;
                else n.ready = false;

                //Add reservation station to queue
                nextEntries.push_back(n);

                //remove taken instruction from issued
                issuedNext->erase(issuedNext->begin() + i);
                // idrs->active = false;
            }
            else {
                std::cout << "ERROR: Tried to issue instruction to RS which is full." << std::endl;
            }
        }
    }
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
    // currentEntries.clear();
    // for(int i = 0; i < nextEntries.size(); i++) ReservationStation::currentEntries[i] = ReservationStation::nextEntries[i];
}

ReservationStation::ReservationStation(Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount) {
    ReservationStation::RF = RF;
    ReservationStation::issuedCurrent = issuedCurrent;
    ReservationStation::issuedNext = issuedNext;
    ReservationStation::type = type;
    ReservationStation::RSID = RSID;
    ReservationStation::RSCount = RSCount;
    // ReservationStation::rsex = rsex;
}