#pragma once
#include <vector>
#include "instr.h"
#include "ReservationStation.h"

enum InstrType { REG, LOAD, STORE, BRANCH};

typedef int ROBID;

struct ROBEntry {
    ROBID id;
    InstrType type;
    int result;
    int dest;
    bool ready;
    //bool inUse;
};

class ReorderBuffer {
private:
    Register *RF;
    int32_t *MEM;
    std::vector<ReservationStation> *RSs;

public:
    std::vector<ROBEntry> currentROB;
    std::vector<ROBEntry> nextROB;
    
    ROBID addEntry(Instr i);
    void tick();
    void update();
    ReorderBuffer() {};
    ReorderBuffer(Register *RF, int32_t *MEM, std::vector<ReservationStation> *RSs);
};