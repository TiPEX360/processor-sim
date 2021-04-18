#include "DecodeUnit.h"

DecodeUnit::DecodeUnit(int *RF, Instr* cir, PipelineRegister *ifid, PipelineRegister *idex) {
    DecodeUnit::cir = cir;
    DecodeUnit::ifid = ifid;
    DecodeUnit::idex = idex;
    DecodeUnit::RF = RF;
}

int DecodeUnit::decode() {
    //Copy state
    *idex = *ifid;

    Instr i = ifid->cir;
    //Register fetch
    if(i.opcode == ST || (i.opcode >= BLT && i.opcode <= B)) idex->Rd = RF[i.Rd];
    else idex->Rd = i.Rd;
    idex->Rn = RF[i.Rn];
    if(i.immediate) idex->Ri = i.Ri;
    else idex->Ri = RF[i.Ri];

    ifid->active = false;
    idex->active = true;

    return 0;
}