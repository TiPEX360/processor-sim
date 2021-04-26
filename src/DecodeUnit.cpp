#include "DecodeUnit.h"
#include <iostream>
DecodeUnit::DecodeUnit(int *RF, Instr* cir, PipelineRegister *ifid, PipelineRegister *idex) {
    DecodeUnit::cir = cir;
    DecodeUnit::ifid = ifid;
    DecodeUnit::idex = idex;
    DecodeUnit::RF = RF;
}

int DecodeUnit::decode() {
    //Copy state
    if(idex->cond) {

        idex->cond = false;

        idex->active = false;
        ifid->active = false;
        return 1;
    }

    (*idex) = (*ifid);




    Instr i = ifid->cir;

    std::cout << "DECODE// Rd: " << (int)i.Rd << " Rn: " << (int)i.Rn << " Ri: " << (int)i.Ri << std::endl;
    if(i.opcode == JNZ) {
        for(int i = 0; i < 31; i++) {
            std::cout << "RF[" << i << "] " << RF[i] << std::endl;
        }
    }

    //Register fetch
    if((i.opcode == ST) || (i.opcode >= BLT && i.opcode <= B)) idex->Rd = RF[i.Rd];
    else idex->Rd = i.Rd;
    idex->Rn = RF[i.Rn];
    if(i.immediate) idex->Ri = i.Ri;
    else idex->Ri = RF[i.Ri];
    // std::cout << i.Ri << std::endl;
    ifid->active = false;
    idex->active = true;

    std::cout << "DECODE// Op: " << idex->cir.opcode << " Rd: " << idex->Rd << " Rn: " << idex->Rn << " Ri: " << idex->Ri << std::endl;
    return 0;
}