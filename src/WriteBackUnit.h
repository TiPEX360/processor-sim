#pragma once
#include "instr.h"
#include "PipelineRegister.h"

class WriteBackUnit {
private:
    int *RF;
    PipelineRegister *memwb;
public:
    WriteBackUnit(int *RF, PipelineRegister *memwb);
    int wb();
};