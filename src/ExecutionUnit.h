#pragma once
#include "instr.h"
#include <iostream>

class ExecutionUnit {
private: 
    int *MEM;
    int *RF;
    int *pc;
    bool *halt;

public:
    ExecutionUnit(bool *halt, int *MEM, int *RF, int *pc);

    int execute(Instr instr);
};