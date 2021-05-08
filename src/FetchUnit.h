#pragma once
#include "instr.h"
#include <iostream>
#include "BPB.hpp"
#include <queue>
#include <array>
#include <vector>

class FetchUnit {
private:
    Register *currentPC;
    Register *nextPC;
    Instr *INSTR;
    BPB *branchBuffer;

    Instr initInstr(Instr n);
public:

    std::array<std::queue<Instr>, 4> currentFetched;
    std::array<std::queue<Instr>, 4> nextFetched;


    FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR);
    // int fetch();
    // void before();
    void tick();
    void update();

    FetchUnit();
};