#pragma once
#include "instr.h"
#include <iostream>
#include "PipelineRegister.h"

class ExecutionUnit {
private: 
    bool *halt;
    PipelineRegister *idex;
    PipelineRegister *exmem;

public:
    ExecutionUnit(bool *halt, PipelineRegister *idex, PipelineRegister *exmem);

    int execute();
};