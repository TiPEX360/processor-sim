#pragma once
#include "instr.h"

class DecodeUnit {
private:
    Instr *cir;
public:
    DecodeUnit(Instr * cir);
    int decode() ;
};