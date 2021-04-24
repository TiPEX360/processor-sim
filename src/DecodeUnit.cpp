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
    (*idex) = (*ifid);
    idex->active = false;

    Instr i = ifid->cir;
    std::cout << "Op: " << i.opcode << " Rd: " << (int)i.Rd << " Rn: " << (int)i.Rn << " Ri: " << (int)i.Ri << std::endl;
    //Register fetch
    if((i.opcode == ST) || (i.opcode >= BLT && i.opcode <= B)) idex->Rd = RF[i.Rd];
    else idex->Rd = i.Rd;
    idex->Rn = RF[i.Rn];
    if(i.immediate) idex->Ri = i.Ri;
    else idex->Ri = RF[i.Ri];
    std::cout << i.Ri << std::endl;
    ifid->active = false;
    idex->active = true;

    return 0;
}