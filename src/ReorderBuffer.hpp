#pragma once
#include <vector>
#include <queue>
#include "instr.h"
#include "BPB.hpp"
// #include "ReservationStation.h"
// #include "ExecutionUnit.h"

// class ReservationStation;
class ExecutionUnit;
class ReservationStation;

typedef int ROBID;

const int ROB_MAX = 128;


class ReorderBuffer {
private:
    Register *nextRF;
    int32_t *nextMEM;
    std::array<ReservationStation, RS_COUNT> *RSs;
    std::array<ExecutionUnit *, EXEC_COUNT> *EUs;
    std::queue<Instr> *nextFetched;
    BPB *branchBuffer;
    bool *halt;
public:
    std::vector<ROBEntry> currentROB;
    std::vector<ROBEntry> nextROB;

    int updateEntry(int index, ROBEntry e);
    ROBID addEntry(RSEntry RSe, RSID RSID, int branchTaken);
    void flush(ROBEntry branchEntry);
    RSID findRSIDByROBEntry(ROBEntry e);
    void tick();
    void update();
    ReorderBuffer() {};
    ReorderBuffer(BPB *branchBuffer, std::queue<Instr> *nextFetched, bool *halt, std::array<ExecutionUnit *, EXEC_COUNT> *EUs, Register *nextRF, int32_t *nextMEM, std::array<ReservationStation, RS_COUNT> *RSs);
};