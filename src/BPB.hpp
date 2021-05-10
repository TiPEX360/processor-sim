#pragma once
#include <array>
#include "instr.h"

struct BPBEntry {
    char state; 
    // bool initialised = false;
};


class BPB {
private:
    std::array<BPBEntry, 16> currentBuffer;
    std::array<BPBEntry, 16> nextBuffer;

public:
    bool predictBranchDynamic(uint32_t pc, Instr i, BPType type);
    void updateBranch(uint32_t pc, bool taken);
    void update();
    BPB();
};