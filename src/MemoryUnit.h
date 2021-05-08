#pragma once
#include "instr.h"
#include "PipelineRegister.h"

class MemoryUnit {
private:
    int *MEM;
    PipelineRegister *exmem;
    PipelineRegister *memwb;
    int *pc;
public:
    MemoryUnit(int *pc, int *MEM, PipelineRegister *exmem, PipelineRegister *memwb);
    int memory();
};