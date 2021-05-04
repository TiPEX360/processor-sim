#pragma once
#include "instr.h"
#include <iostream>
#include "PipelineRegister.h"
#include <queue>
#include <vector>


class FetchUnit {

private:
    Register *pc;
    Instr *INSTR;
    PipelineRegister *ifid;
    PipelineRegister *exmem;
public:

    std::queue<Instr> current;
    std::queue<Instr> next;


    FetchUnit(Register *pc, Instr *INSTR);
    // int fetch();
    // void before();
    void tick();
    void update();

    FetchUnit();
};