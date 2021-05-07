#pragma once
#include <vector>
#include "instr.h"
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
    bool *halt;
public:
    std::vector<ROBEntry> currentROB;
    std::vector<ROBEntry> nextROB;

    int updateEntry(int index, ROBEntry e);
    ROBID addEntry(RSEntry RSe, RSID RSID, int branchTaken);
    void flush();
    RSID findRSIDByROBEntry(ROBEntry e);
    void tick();
    void update();
    ReorderBuffer() {};
    ReorderBuffer(bool *halt, std::array<ExecutionUnit *, EXEC_COUNT> *EUs, Register *nextRF, int32_t *nextMEM, std::array<ReservationStation, RS_COUNT> *RSs);
};