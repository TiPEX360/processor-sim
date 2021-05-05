#pragma once
#include <vector>
#include "instr.h"
#include "ReservationStation.h"

enum InstrType { REG, MEM, BRANCH};

typedef int ROBID;

const int ROB_MAX = 128;

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
    int currentOccupied = 0;
    int nextOccupied = 0;

    ROBID addEntry(RSEntry RSe);
    void tick();
    void update();
    ReorderBuffer() {};
    ReorderBuffer(Register *RF, int32_t *MEM, std::vector<ReservationStation> *RSs);
};