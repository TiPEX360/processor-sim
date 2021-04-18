#pragma once
#include "instr.h"
#include "PipelineRegister.h"

class MemoryUnit {
private:
    int *MEM;
    PipelineRegister *exmem;
    PipelineRegister *memwb;
public:
    MemoryUnit(int *MEM, PipelineRegister *exmem, PipelineRegister *memwb);
    int memory();
};