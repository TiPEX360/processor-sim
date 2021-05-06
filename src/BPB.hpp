#pragma once
#include <array>
#include "instr.h"

struct BPBEntry {
    char state : 2; 
    // bool initialised = false;
};


class BPB {
private:
    std::array<BPBEntry, 16> currentBuffer;
    std::array<BPBEntry, 16> nextBuffer;

public:
    bool predictBranchDynamic(uint32_t pc, Instr i);
    void updateBranch();
    BPB();
};