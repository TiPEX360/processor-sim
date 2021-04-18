#pragma once
#include "instr.h"
#include "PipelineRegister.h"

class DecodeUnit {
private:
    Instr *cir;
    PipelineRegister *ifid;
    PipelineRegister *idex;
    int *RF;

public:
    DecodeUnit(int *RF, Instr * cir, PipelineRegister *ifid, PipelineRegister *idex);
    int decode();
};