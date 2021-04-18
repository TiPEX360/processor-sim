#pragma once
#include "instr.h"
#include <iostream>
#include "PipelineRegister.h"

class FetchUnit {
private:
    int *pc;
    Instr *INSTR;
    PipelineRegister *ifid;
    PipelineRegister *exmem;

public:
    FetchUnit(int *pc, Instr *INSTR, PipelineRegister *ifid, PipelineRegister *exmem);
    int fetch();
};