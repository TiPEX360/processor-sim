#pragma once
#include "instr.h"
#include <iostream>
#include "BPB.hpp"
#include <queue>
#include <vector>


class FetchUnit {

private:
    Register *currentPC;
    Register *nextPC;
    Instr *INSTR;
    BPB *branchBuffer;
public:

    std::queue<Instr> currentFetched;
    std::queue<Instr> nextFetched;


    FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR);
    // int fetch();
    // void before();
    void tick();
    void update();

    FetchUnit();
};