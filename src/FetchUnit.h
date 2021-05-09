#pragma once
#include "instr.h"
#include <iostream>
#include "BPB.hpp"
#include <queue>
#include <array>
#include <deque>
#include <vector>

class FetchUnit {
private:
    Register *currentPC;
    Register *nextPC;
    Instr *INSTR;
    BPB *branchBuffer;

    Instr initInstr(Instr n);
public:

    std::deque<Instr> currentFetched;
    std::deque<Instr> nextFetched;


    FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR);
    // int fetch();
    // void before();
    void tick();
    void update();

    FetchUnit();
};