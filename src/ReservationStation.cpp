#include "ReservationStation.h"

void ReservationStation::tick() {
    //Add potential new instruction to RS queue
    if(idrs->active) {
        if(current.size() < RSCount) {
            RSData n;
            Instr i = idrs->cir;
            n.busy = true;
            n.opcode = i.opcode;

            //Rd
            if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
                if(RF[i.Rd].RS == -1) n.Rd = RF[i.Rd].value;
            }
            else n.Rd = i.Rd;
            //Rn
            if(RF[i.Rn].RS == -1) n.Rn = RF[i.Rn].value;
            //Ri
            if(i.immediate) n.Ri = i.Ri;
            else if(RF[i.Ri].RS == -1) n.Ri = RF[i.Ri].value;

            //RSd
            if((n.opcode == ST) || (n.opcode >= BLT && n.opcode <= B)) {
                n.RSd = RF[n.Rd].RS;
            }
            else n.RSd = -1;
            //RSn
            n.RSd = RF[i.Rn].RS;
            //RSi
            if(i.immediate) n.RSn = -1;
            else n.RSn = RF[i.Ri].RS;

            if(n.RSd == -1 && n.RSn == -1 && n.RSi == -1) n.ready = true;
            else n.ready = false;

            //Add reservation station to queue
            next.push_back(n);

            //flag taken instruction
            idrs->active = false;
        }


    }


}

void ReservationStation::update() {
    for(int i = 0; i < current.size(); i++) ReservationStation::current[i] = ReservationStation::next[i];
}

ReservationStation::ReservationStation(Register *RF, PipelineRegister *idrs, int RSCount) {
    ReservationStation::RF = RF;
    ReservationStation::idrs = idrs;
    ReservationStation::RSCount = RSCount;
    // ReservationStation::rsex = rsex;
}